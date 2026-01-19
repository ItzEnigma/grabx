#pragma once
#include <optional>
#include <string>

namespace db::model
{
class Download
{
public:
    Download(const std::string& url,
             const std::string& status,
             double progress,
             const std::optional<long long>& totalSize,
             const std::optional<long long>& downloadedSize,
             const std::optional<std::string>& filePath,
             const std::string& createdAt,
             const std::string& updatedAt)
        : _url(url), _status(status), _progress(progress), _totalSize(totalSize), _downloadedSize(downloadedSize),
          _filePath(filePath), _createdAt(createdAt), _updatedAt(updatedAt)
    {
    }

    /********** Getters **********/
    int getId() const { return _id; }
    const std::string& getUrl() const { return _url; }
    const std::string& getStatus() const { return _status; }
    double getProgress() const { return _progress; }
    const std::optional<long long>& getTotalSize() const { return _totalSize; }
    const std::optional<long long>& getDownloadedSize() const { return _downloadedSize; }
    const std::optional<std::string>& getFilePath() const { return _filePath; }
    const std::string& getCreatedAt() const { return _createdAt; }
    const std::string& getUpdatedAt() const { return _updatedAt; }
    /********** Setters **********/
    void setId(int id) { _id = id; }
    void setUrl(const std::string& url) { _url = url; }
    void setStatus(const std::string& status) { _status = status; }
    void setProgress(double progress) { _progress = progress; }
    void setTotalSize(const std::optional<long long>& size) { _totalSize = size; }
    void setDownloadedSize(const std::optional<long long>& size) { _downloadedSize = size; }
    void setFilePath(const std::optional<std::string>& path) { _filePath = path; }
    void setUpdatedAt(const std::string& time) { _updatedAt = time; }

private:
    int _id = -1;
    std::string _url;
    std::string _status;
    double _progress;
    std::optional<long long> _totalSize;
    std::optional<long long> _downloadedSize;
    std::optional<std::string> _filePath;
    std::string _createdAt;
    std::string _updatedAt;
};

} // namespace db::model
