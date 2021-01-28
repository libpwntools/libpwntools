#include <pwntools>
#include <iostream>
#include <vector>
#include <algorithm>

Remote io;
std::string get_flag() {
    io.sendlineafter(": ", "1");
    std::string rcv = io.recvline();
    return hex_to_string(remove_newline(rcv));
}

std::string encrypt(const std::string &message) {
    io.sendlineafter(": ", "2");
    io.sendlineafter(": ", string_to_hex(message));
    std::string rcv = io.recvline();
    return hex_to_string(remove_newline(rcv));
}

int main(int argc, char ** argv) {
    if(argc != 3) {
        std::cout << argv[0] << " <remote_ip> <remote_port>\n";
        return 1;
    }
    
    io = Remote(argv[1], atoi(argv[2]));
    std::string enc_flag = get_flag();
    size_t len = enc_flag.length();
    std::vector<unsigned int> enc_iv[len];
    
    //io.set_debug(true);

    for(int i=0; i<len; ++i)
        for(int j=0;j<0x100;++j)
            enc_iv[i].push_back(j);

    std::string null(len, 0);
    bool run;

    while(true) {
        run = false;
        for(auto chars : enc_iv)
            if(chars.size() > 1)
                run = true;
        if(!run) break;
        std::string enc_pt(len, 0); 

        enc_pt = encrypt(enc_pt);
        enc_pt = str_xor(enc_pt, null);

        for(int i=0; i<len; ++i) {
            auto pos = std::find(enc_iv[i].begin(), enc_iv[i].end(), (unsigned char)enc_pt[i]);
                if(pos != enc_iv[i].end())
                    enc_iv[i].erase(pos);
        }
    }

    std::vector<unsigned int> flag[len];

    for(int i=0; i<len; ++i) {
        for(int j=0;j<0x100;++j) {
            if((j ^ enc_iv[i][0]) >= 0x20 && (j ^ enc_iv[i][0]) < 0x7f)
                flag[i].push_back(j);
        }
    }

    while(true) {
        run = false;
        for(auto chars : flag)
            if(chars.size() > 1)
                run = true;
        if(!run) break;

        enc_flag = get_flag();
        for(int i=0; i<len; ++i) {
            auto pos = std::find(flag[i].begin(), flag[i].end(), (unsigned char)enc_flag[i]);
                if(pos != flag[i].end())
                    flag[i].erase(pos);
        }

    }

    for(int i=0; i<len; ++i)
        std::cout << (char)(flag[i][0] ^ enc_iv[i][0]);
    std::cout << '\n';
}
