#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <direct.h> 

class MorseSystem {
private:
    std::map<char, std::string> charToMorse;
    std::map<std::string, char> morseToChar;

    void addMapping(char c, std::string standard) {
        // 存储用于音频逻辑的原始点划
        charToMorse[tolower(c)] = standard;
        
        // 生成用于解密的变种电码对照表 (例如: ".-" -> "liqui")
        std::string variant = "";
        for(size_t i=0; i<standard.length(); ++i) {
            variant += (standard[i] == '.' ? "li" : "qui");
        }
        morseToChar[variant] = tolower(c);
    }

    void init() {
        addMapping('a', ".-");   addMapping('b', "-..."); addMapping('c', "-.-.");
        addMapping('d', "-..");  addMapping('e', ".");    addMapping('f', "..-.");
        addMapping('g', "--.");  addMapping('h', "...."); addMapping('i', "..");
        addMapping('j', ".---"); addMapping('k', "-.-");  addMapping('l', ".-..");
        addMapping('m', "--");   addMapping('n', "-.");   addMapping('o', "---");
        addMapping('p', ".--."); addMapping('q', "--.-"); addMapping('r', ".-.");
        addMapping('s', "...");  addMapping('t', "-");    addMapping('u', "..-");
        addMapping('v', "...-"); addMapping('w', ".--");  addMapping('x', "-..-");
        addMapping('y', "-.--"); addMapping('z', "--..");
        addMapping('1', ".----"); addMapping('2', "..---"); addMapping('3', "...--");
        addMapping('4', "....-"); addMapping('5', "....."); addMapping('6', "-....");
        addMapping('7', "--..."); addMapping('8', "---.."); addMapping('9', "----.");
        addMapping('0', "-----");
    }

public:
    MorseSystem() { init(); }

    // --- 加密与合成功能 ---
    void encryptAndAudio() {
        std::string text;
        std::cout << "请输入原文: ";
        std::getline(std::cin, text);

        _mkdir("output");
        std::vector<std::string> audioFiles;
        std::string encodedText = "";

        for (size_t i = 0; i < text.length(); ++i) {
            char ch = tolower(text[i]);
            if (ch == ' ') {
                encodedText += " "; 
            } else if (charToMorse.count(ch)) {
                std::string code = charToMorse[ch];
                for (size_t j = 0; j < code.length(); ++j) {
                    if (code[j] == '.') {
                        encodedText += "li";
                        audioFiles.push_back("li.wav");
                    } else {
                        encodedText += "qui";
                        audioFiles.push_back("qui.wav");
                    }
                }
                if (i + 1 < text.length() && text[i+1] != ' ') {
                    encodedText += "d";
                    audioFiles.push_back("d.wav");
                }
            }
        }

        std::cout << "\n[加密密文]: " << encodedText << std::endl;

        std::ofstream listFile("output/list.txt");
        for (size_t k = 0; k < audioFiles.size(); ++k) 
            listFile << "file '../" << audioFiles[k] << "'\n";
        listFile.close();

        std::cout << "[提示]: 正在合成音频..." << std::endl;
        system("ffmpeg -f concat -safe 0 -i output/list.txt -y -c copy output/output.wav > nul 2>&1");
        std::cout << "[完成]: 结果已存至 output/output.wav\n" << std::endl;
    }

    // --- 解密功能 ---
    void decrypt() {
        std::string code;
        std::cout << "请输入密文 (字母间用d分隔，单词间用空格): ";
        std::getline(std::cin, code);

        std::string result = "";
        std::stringstream ssWords(code);
        std::string word;

        while (ssWords >> word) { // 按空格切分单词
            std::stringstream ssLetters(word);
            std::string letterSegment;
            
            // 按 'd' 切分字母
            while (std::getline(ssLetters, letterSegment, 'd')) {
                if (morseToChar.count(letterSegment)) {
                    result += morseToChar[letterSegment];
                }
            }
            result += " ";
        }
        std::cout << "\n[解密原文]: " << result << "\n" << std::endl;
    }
};

int main() {
    MorseSystem sys;
    std::string choice;

    while (true) {
        std::cout << "======= 一种基于老蛇大叫LIQUIIIIIIIID的变种摩尔斯电码 =======" << std::endl;
        std::cout << "1. 加密文本并生成音频\n2. 解密密文\n3. 退出程序" << std::endl;
        std::cout << "请选择操作 (1-3): ";
        std::getline(std::cin, choice);

        if (choice == "1") sys.encryptAndAudio();
        else if (choice == "2") sys.decrypt();
        else if (choice == "3") break;
        else std::cout << "输入无效，请重新选择。" << std::endl;
        
        std::cout << "按回车键继续..." << std::endl;
        std::cin.get();
        system("cls"); // 清屏，让界面整洁
    }

    return 0;
}
