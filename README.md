# *LSB Steganography*
A project that implements Least Significant Bit (LSB) Steganography to securely hide data within image files. This technique allows for imperceptible embedding of secret messages in images.

## *Table of Contents*
- [How It Works](#how-it-works)
- [Features](#features)
- [Workflow](#workflow)
- [Setup and Usage](#setup-and-usage)
- [Example Output](#example-output)
- [License](#license)

---

## *How It Works*
LSB Steganography modifies the least significant bits of the image's pixel values to encode secret information. These changes are so subtle that they are visually undetectable.

### *Key Steps:*
1. *Encoding:*
   - Convert secret data to binary.
   - Modify the image's pixel values to embed the binary data.

2. *Decoding:*
   - Read the modified pixel values.
   - Extract the binary data and reconstruct the original message.

---

## *Features*
- Supports encoding text data into images.
- Outputs a stego-image with hidden data.
- Decodes and retrieves the hidden data from the stego-image.

---

## *Workflow*

### *1. Input Phase*
Provide:
- An image file (e.g., .png, .bmp).
- A secret message or file to hide.

### *2. Encoding Process*
- *Step 1:* Convert the secret message into binary.
- *Step 2:* Modify the least significant bits of pixel values to embed the binary data.
- *Step 3:* Save the resulting image as an encoded image file.

### *3. Decoding Process*
- *Step 1:* Load the encoded image.
- *Step 2:* Extract the binary data from the least significant bits.
- *Step 3:* Reconstruct and display the original hidden message.

---
## *Example Output*

### *Input Image:*
(Original Image)  
![Input Image](Steganography project/LSB Stegnography project/4-SkeletonCode/beautiful.bmp)

### *Encoded Image:*
(Image with hidden data — visually identical to the input image)  
![Encoded Image](Steganography project/LSB Stegnography project/4-SkeletonCode/default_stego_img.bmp)

### *Extracted Data:*
```plaintext
"This is a secret message!"
