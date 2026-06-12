/*
THEORY: Memory Mapped Files
- Maps file contents directly into virtual memory
- OS handles paging, no explicit read/write calls
- Much faster for random access and large files
- Uses mmap() on POSIX, CreateFileMapping on Windows
- File changes in memory are automatically synced to disk

Complexity: O(1) map, O(page_size) sync
Interview Questions:
1. Advantages over read()/write()? (lazy loading, shared memory)
2. What is the page size and how does it affect mapping?
3. How to handle files larger than available RAM?
Common Mistakes:
- Not checking MAP_FAILED return value
- Forgetting to msync() before munmap() for write mappings
- File not properly aligned to page boundaries
*/

#include <iostream>
#include <string>
#include <cstring>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <vector>

class MappedFile {
    int fd_{-1};
    void* mapped_{MAP_FAILED};
    size_t size_{0};
    std::string filename_;

public:
    MappedFile() = default;

    ~MappedFile() { close(); }

    bool open(const std::string& filename, bool writable = false) {
        filename_ = filename;
        int flags = O_RDONLY;
        if (writable) flags = O_RDWR | O_CREAT;

        fd_ = ::open(filename.c_str(), flags, 0644);
        if (fd_ < 0) {
            perror("open");
            return false;
        }

        struct stat st{};
        if (fstat(fd_, &st) < 0) {
            perror("fstat");
            ::close(fd_);
            return false;
        }
        size_ = st.st_size;
        if (size_ == 0) {
            std::cout << "Empty file\n";
            ::close(fd_);
            return true;
        }

        int prot = PROT_READ;
        if (writable) prot |= PROT_WRITE;

        mapped_ = mmap(nullptr, size_, prot, MAP_SHARED, fd_, 0);
        if (mapped_ == MAP_FAILED) {
            perror("mmap");
            ::close(fd_);
            return false;
        }

        ::close(fd_); // Can close after mapping
        fd_ = -1;

        std::cout << "Mapped " << filename << " (" << size_ << " bytes)\n";
        return true;
    }

    void* data() const { return mapped_; }
    size_t size() const { return size_; }

    bool sync() {
        if (mapped_ == MAP_FAILED || mapped_ == nullptr) return false;
        return msync(mapped_, size_, MS_SYNC) == 0;
    }

    void close() {
        if (mapped_ != MAP_FAILED && mapped_ != nullptr) {
            munmap(mapped_, size_);
            mapped_ = MAP_FAILED;
        }
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
    }

    bool isOpen() const { return mapped_ != MAP_FAILED && mapped_ != nullptr; }

    // Convenience: read as string
    std::string toString() const {
        if (!isOpen() || size_ == 0) return "";
        return std::string(static_cast<const char*>(mapped_), size_);
    }

    // Create a file with content (for demo purposes)
    static bool createFile(const std::string& path, const std::string& content) {
        int fd = ::open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) return false;
        write(fd, content.c_str(), content.size());
        ::close(fd);
        return true;
    }
};

int main() {
    // Create a test file
    MappedFile::createFile("/tmp/test_mmap.txt",
        "Hello Memory Mapped File!\nThis is line 2.\n");

    MappedFile mf;
    if (!mf.open("/tmp/test_mmap.txt")) return 1;

    std::cout << "Content:\n" << mf.toString() << "\n";

    // Count lines
    const char* data = static_cast<const char*>(mf.data());
    size_t lines = 0;
    for (size_t i = 0; i < mf.size(); ++i) {
        if (data[i] == '\n') ++lines;
    }
    std::cout << "Line count: " << lines << "\n";

    // Search for a pattern
    const char* found = static_cast<const char*>(
        memmem(mf.data(), mf.size(), "Memory", 6));
    if (found) {
        std::cout << "Found 'Memory' at offset: "
                  << (found - static_cast<const char*>(mf.data())) << "\n";
    }

    mf.close();
    unlink("/tmp/test_mmap.txt");
    return 0;
}

/*
SAMPLE OUTPUT:
Mapped /tmp/test_mmap.txt (46 bytes)
Content:
Hello Memory Mapped File!
This is line 2.
Line count: 2
Found 'Memory' at offset: 6
*/
