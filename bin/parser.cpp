#include "parser.h"

void parse(const std::vector<std::string>& arguments) {
    const std::string help = R"(
This is archive programm, it can take files and encode them.
To create new archive, type "-c" or "--create", dont forget give name your archive by typing "-f ARCHIVE_NAME" or "--file=ARCHIVE_NAME"
also you can give files immediatelly after creating archive by typing them names (after archive name) separating with a space button like this: "FILE1 FILE2 FILE3"
To add argument for encoding which means length of the information block type "-i X" (where X is integer > 0) or "--information X". you need write when you create archive, else X will set by default (8).
To see all added files in archive, type "-l ARCHIVE_NAME" or "--list ARCHIVE_NAME"
To extract files from archive, type "-x FILE1 FILE2 FILE3 ARCHIVENAME" or "--extract FILE1 FILE2 FILE3 ARCHIVENAME", also you can just type "-x ARCHIVENAME" or "--extract ARCHIVENAME" and ALL files from archive will be extracted
To append file to archive, type "-a FILE1 ARCHIVENAME" or "--append FILE1 ARCHIVENAME"
To delete file from archive, type "-d FILE1 ARCHIVENAME" or "--delete FILE1 ARCHIVENAME"
To merge two archive, type "-A ARCHIVE1 ARCHIVE2 -f ARCHIVE3" or "--concatenate ARCHIVE1 ARCHIVE2 -f ARCHIVE3

Examples: hamarc --create --file=ARCHIVE -i 4 FILE1 FILE2 FILE3
          hamarc -l -f ARCHIVE --information 15
          hamarc --extract -f ARCHIVE FILE1 FILE2 FILE3
          hamarc --append --file=ARCHIVE FILE5
          hamarc -d -f ARCHIVE FILE3
          hamarc --list --file=ARCHIVE
          hamarc --concantenate ARCHIVE1 ARCHIVE2 -f ARCHIVE3 -i 13
)";
    if (arguments.empty()) {
        std::cout << help << '\n';
    }
    int message_len = 8;
    std::string archive_name = "ARCHIVE";
    std::vector<std::string> add_me;
    std::vector<std::string> extract_me;
    std::vector<std::string> delete_me;
    std::string first_archive;
    std::string second_archive;

    int i = 0;
    if (arguments[i] == "--help" || arguments[i] == "-h" || arguments[i] == "help" || arguments[i] == "h") {
        std::cout << help << '\n';
        return;
    }
    if (arguments[i] == "-c" || arguments[i] == "--create") {
        i++;
        if (arguments[i] == "-f") {
            archive_name = arguments[i + 1];
            i+=2;
        } else if (arguments[i].find("--file=") == 0) {
            archive_name = arguments[i].substr(1 + arguments[i].find_first_of('='));
            i++;
        }
        if (arguments[i] == "-i" || arguments[i] == "--information") {
            message_len = std::stoi(arguments[i+1]);
            i+=2;
        }
        while (i < arguments.size()) {
            add_me.push_back(arguments[i]);
            i++;
        }
        if (archive_name.empty()) {
            std::cout << "Error! No archive name choosed!" << '\n';
            return;
        }
        CreateArchive(archive_name,message_len);
        AddFileToArchive(archive_name,add_me);
        return;
    }
    else if (arguments[i] == "-l" || arguments[i] == "--list") {
        i++;
        if (arguments[i] == "-f") {
            archive_name = arguments[i + 1];
            i+=2;
        } else if (arguments[i].find("--file=") == 0) {
            archive_name = arguments[i].substr(1 + arguments[i].find_first_of('='));
            i++;
        }
        SeeHeaderList(archive_name);
    }
    else if (arguments[i] == "-x" || arguments[i] == "--extract") {
        i++;
        if (arguments[i] == "-f") {
            archive_name = arguments[i + 1];
            i+=2;
        } else if (arguments[i].find("--file=") == 0) {
            archive_name = arguments[i].substr(1 + arguments[i].find_first_of('='));
            i++;
        }
        while (i < arguments.size()) {
            extract_me.push_back(arguments[i]);
            i++;
        }
        if (archive_name.empty()) {
            std::cout << "Error! No archive name choosed!" << '\n';
            return;
        }
        for (const std::string& file_for_extracting : extract_me) {
            ExtractFileFromArchive(archive_name,file_for_extracting);
        }
    }
    else if (arguments[i] == "-a" || arguments[i] == "--append") {
        i++;
        if (arguments[i] == "-f") {
            archive_name = arguments[i + 1];
            i+=2;
        } else if (arguments[i].find("--file=") == 0) {
            archive_name = arguments[i].substr(1 + arguments[i].find_first_of('='));
            i++;
        }
        while (i < arguments.size()) {
            add_me.push_back(arguments[i]);
            i++;
        }
        AddFileToArchive(archive_name,add_me);
    }
    else if (arguments[i] == "-d" || arguments[i] == "--delete") {
        i++;
        if (arguments[i] == "-f") {
            archive_name = arguments[i + 1];
            i+=2;
        } else if (arguments[i].find("--file=") == 0) {
            archive_name = arguments[i].substr(1 + arguments[i].find_first_of('='));
            i++;
        }
        while (i < arguments.size()) {
            delete_me.push_back(arguments[i]);
            i++;
        }
        for (const std::string& file_for_deleting : delete_me) {
            DeleteFileFromArchive(archive_name,file_for_deleting);
        }
    }
    else if (arguments[i] == "-A" || arguments[i] == "--concatenate") {
        i++;
        first_archive = arguments[i];
        i++;
        second_archive = arguments[i];
        if (arguments[i] == "-f") {
            archive_name = arguments[i + 1];
            i+=2;
        } else if (arguments[i].find("--file=") == 0) {
            archive_name = arguments[i].substr(1 + arguments[i].find_first_of('='));
            i++;
        }
        if (arguments[i] == "-i" || arguments[i] == "--information") {
            message_len = std::stoi(arguments[i+1]);
            i+=2;
        }
        Merge(first_archive,second_archive,archive_name,message_len);
    }
}