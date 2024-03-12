# QuickConsole
 
Solution based on this post

https://stackoverflow.com/questions/2754518/how-can-i-write-fast-colored-output-to-console

## About The Code
This implementation takes the form of a singleton to prevent swapping buffers opposed to editing the buffer directly.

My thought process on the functions I added were to simplify and wrap up the complex parts about how the buffer works but the basic idea is that you create a _QuickConsole_ object in another class that may have to display text, then use it the same way you use _Console.Write();_

Call the _GetInstance()_ function to set your reference.

_InitializeBuffer()_ can be skipped but is used to set the size of the buffer, think about it like setting the size of a canvas you’d paint on in terms of characters on screen. This won’t set the actual size of the console. It is defaultly set to the size of the console but will need to be updated if size changes are made. 

_AddToBuffer()_ is where the magic happens. Think of it like _Console.Write()_‘s cooler brother. This function lets you specify the position in the buffer where you want to write, a string or char, and the colors you want. The only odd part is the int layer argument this function takes. It can be used how you want to utilize it but the main functionality of this variable is that when erasing or clearing the buffer you can choose to clear all of it or just a specific layer. I use the layers to contain specific elements in my game that may need to individually disappear while leaving other things on screen, like a pop up message or a menu overlay.

_DisplayBuffer()_ just pushes the buffer to the screen but before it does it combines all layers together in order. Layer 0 will be below all other layers.

_SetCursorPosition()_ works just as _Console.SetCursorPosition()_ would act, it just sets where your text will start writing to.
