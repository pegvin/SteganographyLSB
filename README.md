# MsgHide
Hide Text In Images Using LSB Stegnography Technique

---
## About LSB Stegnography Technique
Computers are basically rocks trained to flip switches, a single switch can represent their state in either ON or OFF, i.e. 1 or 0 condition.

These switches are also known as `Bits`, 1 single `bit` can store only 2 values, a 0 or a 1 but when we put together 8 of these bits together we can suddenly represent 256 different values, this is known as a `byte`. a single `byte` can store 8 `bits`.

every ASCII character can be represented as a `byte`... well these bytes can also be used to represent what is visible on our screen. don't believe me? open the below given image in a new tab and zoom in.

![image](https://user-images.githubusercontent.com/75035219/231860594-6b4e32b6-f186-41be-91f2-f4c87f203b7e.png)

did you see it? yes there are these 3 separately visible colors, Red, Green & Blue... well everything you see on the screen is basically a single "dot" or a `pixel` and each pixel can show 3 different colors, red, green & blue! and that's everything you see on screen is made up of, many many pixels lighted up with different intensities of red, green & blue.

and convinently enough we only require 1 byte to represent any of those 3 color, 0-255 is intensity of each color, the more towards 0 the more the that color is dark, the more towards 255 the more that color is bright and these combinations can be used to show any color using that single pixel.

well guess what, an image is also made up of pixels, with each pixels being r, g, b where each color is 1 byte, so we just need 3 bytes to represent 1 single pixel.

LSB is a acronym for **L**east **S**ignificant **B**it. which as the name suggests means a bit which is least significant in representing a value, take this for example:

let's say we have a number `51` which can be represented as `50 + 1`, not lets remove that `+ 1` from there so we get `50`... the `1` wasn't that significant which is why when we removed it from the number the number was still close to it's original value.

so the LSB Stegnography Technique uses the least siginificant bits of every color, in every pixel, in whole image to store some data which in our case is a text message.

so what we do is go over every byte (8 bits) in the image and modify the Least Significant 2 Bits, which indeed changes the pixel's intensity but it's so insignificant our normal eyes can't even notice it.

since we are only store 2 bits of information in 1 single bytes, this means it will require us 4 bytes to store 1 single byte of our text message, which isn't bad considering the amount of pixels normal images have.

so a HD image take from a smart phone has a resolution of `1080x1920` which means it has `2073600` pixels. considering each pixels has only 3 colors Red, Green & Blue, this gives us 3 bytes per pixel, so we have a total of `2073600 x 3` or `6220800` bytes to work with. since LSB technique requires 4 bytes to store 1 byte of message, we can store a message `1555200 - 1` charaters long, `- 1` is used because we need `1` byte to store a `\0` which is a unique character telling us that the message has ended.

---
## Usage

```
Usage: ./msghide encode/decode [arguments]

[arguments]
   <filename>  Input File To encode/decode message into/from
   <message>   If Encoding, A Message To Encode Is REQUIRED
```

---
## License
this project is Licensed under [BSD 3-Clause License](./LICENSE)

---
# Thanks
