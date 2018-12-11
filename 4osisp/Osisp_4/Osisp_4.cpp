#include "pch.h"
#include "stdafx.h"
#include "ThreadPool.h"

std::vector<char> getFileBytes(std::string filename);

using namespace std;

int main()
{
	const std::string filename = "test/file.txt";
	int threadCount;
	cout << "Input thread count: ";
	cin >> threadCount;
	int partsCount;
	cout << "Input part count: ";
	cin >> partsCount;


	auto bytes = getFileBytes(filename);
	vector<vector<char>> partBytes(partsCount);
	const int partSize = ceil(bytes.size() / static_cast<float>(partsCount));

	ThreadPool threadPool{threadCount};
	for (auto i = 0; i < partsCount; i++)
	{
		auto lastPartDelta = 0;
		if (i == partsCount - 1)
		{
			lastPartDelta = partSize * partsCount - bytes.size();
		}

		partBytes[i] = vector<char>(partSize - lastPartDelta);
		memcpy(partBytes[i].data(), bytes.data() + i * partSize, partSize - lastPartDelta);

		auto pPartBytes = &partBytes[i];
		threadPool.pushRoutine([i, pPartBytes]
		{
			sort(pPartBytes->begin(), pPartBytes->end());
			cout << "Sorting of " + to_string(i + 1) + " part is completed\n";
			cout.flush();
		});
	}
	threadPool.finish();

	list<char> result;
	for (auto i = 0; i < partsCount; i++)
	{
		list<char> stagingBuffer;
		for (auto j = 0; j < partBytes[i].size(); j++)
		{
			stagingBuffer.push_back(partBytes[i][j]);
		}
		result.merge(stagingBuffer);
	}
	cout << "Merging is completed" << endl;


	std::ofstream output("test/result.txt");
	copy(result.begin(), result.end(), ostream_iterator<char>(output));

	return 0;
}

std::vector<char> getFileBytes(const string filename)
{
	ifstream file(filename, ios::ate | ios::binary);

	if (!file.is_open())
	{
		throw runtime_error("Failed to open file:" + filename);
	}

	const auto fileSize = static_cast<size_t>(file.tellg());
	vector<char> stagingBuffer(fileSize);

	file.seekg(0);
	file.read(&stagingBuffer.front(), fileSize);
	file.close();

	return stagingBuffer;
}
