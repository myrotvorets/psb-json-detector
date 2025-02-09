#include <iostream>
#include <string>
#include <psb/json_detector/json_detector.h>

int main()
{
    psb::json_detector detector;

    while (!std::cin.eof()) {
        std::string s;
        std::getline(std::cin, s);
        auto&& generator = detector.process(s);
        for (auto& chunk : generator) {
            std::cout << chunk << "\n";
        }
    }

    return detector.is_finished() ? 0 : 1;
}
