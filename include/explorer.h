#pragma once
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace tui {

// Cross-platform getch
inline int GetChar() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

// Function to show a list of file names and let user select one
inline std::string
SHOW_SELECT_FILES(const std::vector<std::string> &items,
                  const std::string &title = "Select an item") {
    if (items.empty()) {
        std::cerr << "No items to select.\n";
        return "";
    }

    int selected = 0;

    auto render = [&]() {
        system("clear"); // Use "cls" for Windows
        std::cout << title << " (↑ ↓ j k, Enter to select):\n\n";
        for (size_t i = 0; i < items.size(); ++i) {
            if ((int)i == selected)
                std::cout << " > " << items[i] << "\n";
            else
                std::cout << "   " << items[i] << "\n";
        }
    };

    render();

    while (true) {
        int ch = GetChar();
#ifdef _WIN32
        if (ch == 224)
            ch = GetChar(); // Handle arrow keys
#endif
        if (ch == 65 || ch == 'k') { // Up
            if (selected > 0)
                selected--;
        } else if (ch == 66 || ch == 'j') { // Down
            if (selected < (int)items.size() - 1)
                selected++;
        } else if (ch == 10 || ch == 13) { // Enter
            system("clear");
            return items[selected];
        }

        render();
    }
}

} // namespace tui
