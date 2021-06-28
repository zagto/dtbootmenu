#pragma once

#include <string>
#include <optional>

class Entry {
    private:
        bool legacy = false;
        bool valid = false;

        void fitText();

    public:
        std::string title;
        std::string zImage;
        std::string initrd;
        std::string cmdline;
        std::optional<std::string> dtbFile;
        std::string partition;

        /* regular */
        Entry(std::string partition,
              std::string title,
              std::string zImage,
              std::string initrd,
              std::string cmdline,
              std::optional<std::string> dtbFile);
        /* legacy (downstream 3.x kernels) */
        Entry(std::string partition, std::string title, std::string zImage, std::string initrd);

        void run();
};
