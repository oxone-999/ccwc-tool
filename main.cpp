#include<iostream>
#include<filesystem>
#include<system_error>
#include<fstream>
#include<string_view>
#include<algorithm>
#include<cctype>

using namespace std;

constexpr size_t BUFFER_SIZE = 65536;

void process_stream(istream& in,
                    uintmax_t& out_lines, uintmax_t& out_words, 
                    uintmax_t& out_bytes, uintmax_t& out_chars) {
    
    char buffer[BUFFER_SIZE];
    bool in_word = false;

    // Read chunks until the stream is exhausted
    while (in.read(buffer, BUFFER_SIZE) || in.gcount() > 0) {
        size_t count = in.gcount();
        out_bytes += count; // Every character read is a byte

        for (size_t i = 0; i < count; i++) {
            char c = buffer[i];

            // Count Lines
            if (c == '\n') {
                out_lines++;
            }

            // Count Characters (UTF-8 boundary check)
            if ((c & 0xC0) != 0x80) {
                out_chars++;
            }

            // Count Words
            if (isspace(static_cast<unsigned char>(c))) {
                in_word = false;
            } else if (!in_word) {
                in_word = true;
                out_words++;
            }
        }
    }
}

int main(int argc, char* argv[]){
    
    filesystem::path filepath;
    bool is_count_bytes = false;
    bool is_count_lines = false;
    bool is_count_words = false;
    bool is_count_characters = false;
    bool file_provided=false;

    for(int i=1;i<argc;i++){
	string_view arg(argv[i]);

	if(arg == "-c"){
		is_count_bytes = true;
	}else if(arg == "-l"){
		is_count_lines = true;
	}else if(arg == "-w"){
		is_count_words = true;
	}else if(arg == "-m"){
		is_count_characters=true;
	}else if(arg == "-"){
		cerr<<"incorrect argument type"<<endl;
	}else{
		filepath = arg;
		file_provided=true;
	}
    }

    if(!is_count_bytes && !is_count_words && !is_count_lines){
	is_count_bytes = is_count_words = is_count_lines = true;
    }

    istream* input_stream = &cin;
    ifstream file;

    if(file_provided){  

	file.open(filepath, ios::binary);
    

    	if(!file.is_open())
    	{
	    cerr<<"error opening file"<<endl;
	    return 1;
    	}

	input_stream = &file;
    }

    uintmax_t words=0,characters=0,lines=0,bytes=0;

    process_stream(*input_stream,lines,words,bytes,characters);

    if(is_count_bytes && is_count_words && is_count_lines){
        cout<<bytes<<" bytes "<<lines<<" lines "<<words<<" words ";

    }else if(is_count_bytes){
        cout<<bytes<<endl;
    }else if(is_count_lines){
	cout<<lines<<endl;
    }else if(is_count_words){
	cout<<words<<endl;
    }else if(is_count_characters){
	cout<<characters<<endl;
    }

    if(file_provided) cout<<" "<<filepath.string();
    cout<<endl;

    return 0;
}
