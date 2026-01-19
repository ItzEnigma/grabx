#pragma once
#include <string>

namespace db::model
{
class Segment
{
public:
    Segment(int downloadId,
            long long startByte,
            long long endByte,
            long long downloaded,
            double percentage,
            const std::string& status,
            const std::string& createdAt,
            const std::string& updatedAt)
        : _downloadId(downloadId), _startByte(startByte), _endByte(endByte), _downloaded(downloaded),
          _percentage(percentage), _status(status), _createdAt(createdAt), _updatedAt(updatedAt)
    {
    }

    /********** Getters **********/
    int getId() const { return _id; }
    int getDownloadId() const { return _downloadId; }
    long long getStartByte() const { return _startByte; }
    long long getEndByte() const { return _endByte; }
    long long getDownloaded() const { return _downloaded; }
    double getPercentage() const { return _percentage; }
    const std::string& getStatus() const { return _status; }
    const std::string& getCreatedAt() const { return _createdAt; }
    const std::string& getUpdatedAt() const { return _updatedAt; }

    /********** Setters **********/
    void setId(int id) { _id = id; }
    void setDownloadId(int downloadId) { _downloadId = downloadId; }
    void setStartByte(long long startByte) { _startByte = startByte; }
    void setEndByte(long long endByte) { _endByte = endByte; }
    void setDownloaded(long long downloaded) { _downloaded = downloaded; }
    void setPercentage(double percentage) { _percentage = percentage; }
    void setStatus(const std::string& status) { _status = status; }
    void setUpdatedAt(const std::string& time) { _updatedAt = time; }

private:
    int _id = -1;
    int _downloadId;
    long long _startByte;
    long long _endByte;
    long long _downloaded;
    double _percentage;
    std::string _status;
    std::string _createdAt;
    std::string _updatedAt;
};

} // namespace db::model
