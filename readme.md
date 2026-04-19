# Quadtree Compression

A C++ tool that uses a Quadtree structure to compress images by recursively subdividing them based on color variance.

### Compression Visualization
<div align="center">
  <img src="./assets/gif/parrot.gif" width="500" />
</div>
![Mountain Compression](./assets/gif/mountains.gif)

*Transitioning from high threshold (blocky) to low threshold (detailed).*

### How it works
1. **Recursion:** The image is split into four quadrants.
2. **Variance Check:** If the color variance (Mean Squared Error) in a block is higher than the user-defined `threshold`, it splits further.
3. **Leaf Nodes:** Once the variance is low enough, the recursion stops, and the block is filled with the average color of its pixels.
4. **Odd Dimensions:** Includes custom logic to handle non-power-of-two images (e.g., 472px) by correctly managing remainder pixels in the right and bottom quadrants to prevent black line gaps.

### Compression Results (img3.jpg)

| Frame | Threshold | Output Size | Space Saved |
| :--- | :--- | :--- | :--- |
| 0 | 7000 | 661 KB | 95.63% |
| 1 | 3000 | 775 KB | 94.87% |
| 5 | 500 | 4.77 MB | 68.41% |
| 9 | 0 | 8.48 MB | 43.94% |

*Original file size: 15.12 MB*

### Technical Details
- **Language:** C++17
- **Dependencies:** `stb_image.h` and `stb_image_write.h` for pixel handling.
- **Memory Management:** Recursive tree traversal for both construction and memory deallocation.
- **Indexing:** Uses row-major mapping `(y * width + x) * 3` for efficient buffer access.