#include <iostream>
#include <filesystem>
#include <iomanip>
#include <string> // For filename generation
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "src/quadtree.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

using namespace std;

int main()
{
    const char* inPath = "assets/raw/img2.jpg";
    
    std::error_code ec;
    uintmax_t beforeBytes = std::filesystem::file_size(inPath, ec);
    if (ec) {
        printf("Could not read input file size: %s\n", inPath);
        return 1;
    }

    int width, height, channels;
    unsigned char *img = stbi_load(inPath, &width, &height, &channels, 3);
    if (img == NULL) {
        printf("Error in loading the image\n");
        return 1;
    }

 
float thresholds[] = {7000.0f, 3000.0f,1800.0f,1400.0f, 1000.0f, 500.0f, 150.0f, 50.0f, 10.0f, 0.0f};
    int numFrames = 10;

    cout << "--- Starting Batch Processing ---" << endl;
    cout << "Original Size: " << beforeBytes << " bytes\n\n";

    for (int i = 0; i < numFrames; i++) {
        float currentThreshold = thresholds[i];
        
        Node* root = buildingTree(0, 0, width, height, currentThreshold, img, width);

        unsigned char* output_buffer = new unsigned char[width * height * 3];
        renderTree(root, output_buffer, width);

        string outPath = "img21frame_" + to_string(i) + ".jpg";

        int ok = stbi_write_jpg(outPath.c_str(), width, height, 3, output_buffer, 95);
        
        if (ok) {
            uintmax_t afterBytes = std::filesystem::file_size(outPath, ec);
            if (!ec) {
                double savedPct = (100.0 * (double)(beforeBytes - afterBytes) / (double)beforeBytes);
                
                cout << "[Frame " << i << " | Threshold: " << setw(6) << currentThreshold << "] ";
                cout << "Size: " << setw(8) << afterBytes << " bytes | ";
                cout << "Saved: " << fixed << setprecision(2) << savedPct << "%" << endl;
            }
        }

        deleteTree(root);
        delete[] output_buffer;
    }

    stbi_image_free(img);
    
    return 0;
}