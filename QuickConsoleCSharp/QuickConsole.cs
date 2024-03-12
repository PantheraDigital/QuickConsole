using System;
using System.Collections.Generic;
using System.Text;

using Microsoft.Win32.SafeHandles;
using System.IO;
using System.Runtime.InteropServices;


namespace QuickConsoleCSharp
{
    public struct Vector2
    {
        public int x;
        public int y;

        public Vector2(int xPos, int yPos)
        {
            x = xPos;
            y = yPos;
        }

        public bool IsEqual(Vector2 other)
        {
            if (other.x == x && other.y == y)
                return true;
            else
                return false;
        }
    }


    public class QuickConsole
    {
        [DllImport("Kernel32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        static extern SafeFileHandle CreateFile(
        string fileName,
        [MarshalAs(UnmanagedType.U4)] uint fileAccess,
        [MarshalAs(UnmanagedType.U4)] uint fileShare,
        IntPtr securityAttributes,
        [MarshalAs(UnmanagedType.U4)] FileMode creationDisposition,
        [MarshalAs(UnmanagedType.U4)] int flags,
        IntPtr template);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteConsoleOutputW(
            SafeFileHandle hConsoleOutput,
            CharInfo[] lpBuffer,
            Coord dwBufferSize,
            Coord dwBufferCoord,
            ref SmallRect lpWriteRegion);

        [StructLayout(LayoutKind.Sequential)]
        public struct Coord
        {
            public short X;
            public short Y;

            public Coord(short X, short Y)
            {
                this.X = X;
                this.Y = Y;
            }
        };

        [StructLayout(LayoutKind.Explicit)]
        public struct CharUnion
        {
            [FieldOffset(0)] public ushort UnicodeChar;
            [FieldOffset(0)] public byte AsciiChar;
        }

        [StructLayout(LayoutKind.Explicit)]
        public struct CharInfo
        {
            [FieldOffset(0)] public CharUnion Char;
            [FieldOffset(2)] public short Attributes;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SmallRect
        {
            public short Left;
            public short Top;
            public short Right;
            public short Bottom;
        }

        public struct CharSetInfo
        {
            public bool set;
            public CharInfo charInfo;
        }

        static QuickConsole instance;

        static SafeFileHandle handle;
        static List<List<CharSetInfo>> bufList;
        static SmallRect rect;
        static Vector2 cursorPosition;
        static short bufWidth;
        static short bufHeight;

        //singlton
        private QuickConsole()
        {
            InitializeBuffer();
        }

        static public QuickConsole GetInstance()
        {
            if (instance == null)
                instance = new QuickConsole();

            return instance;
        }

        public static bool InitializeBuffer(short bufferX = 0, short bufferY = 0)
        {

            if (bufferX <= 0 || bufferY <= 0)
            {
                bufferX = (short)Console.WindowWidth;
                bufferY = (short)Console.WindowHeight;
            }

            cursorPosition = new Vector2(0, 0);

            handle = CreateFile("CONOUT$", 0x40000000, 2, IntPtr.Zero, FileMode.Open, 0, IntPtr.Zero);

            if (!handle.IsInvalid)
            {
                List<CharSetInfo> temp = new List<CharSetInfo>(new CharSetInfo[bufferX * bufferY]);
                bufList = new List<List<CharSetInfo>>();
                bufList.Add(temp);

                rect = new SmallRect() { Left = 0, Top = 0, Right = bufferX, Bottom = bufferY };
                bufWidth = bufferX;
                bufHeight = bufferY;

                return true;
            }
            else
                return false;

        }

        bool ValidLayer(int layer)
        {
            if (layer >= 0 && layer < bufList.Count)
                return true;
            else
                return false;
        }

        bool ValidCursorPosition(Vector2 position)
        {
            if (position.x >= 0 && position.x < bufWidth)
            {
                if (position.y >= 0 && position.y < bufHeight)
                {
                    return true;
                }
            }

            return false;
        }

        int GetBufferPos(int x, int y, int layer)
        {
            if (ValidLayer(layer))
                return x + ((bufList[layer].Count / bufHeight) * y);

            return -1;
        }

        public bool SetCursorPosition(Vector2 position)
        {
            if (ValidCursorPosition(position))
            {
                cursorPosition = position;
                return true;
            }

            return false;
        }

        public void AddToBuffer(int layer, char input, ConsoleColor foreground = ConsoleColor.White, ConsoleColor background = ConsoleColor.Black)
        {
            if (!ValidCursorPosition(cursorPosition))
            {
                return;
            }

            if (!ValidLayer(layer))
            {
                //add missing layers
                int layersToAdd = layer - bufList.Count + 1;
                for (int i = 0; i < layersToAdd; ++i)
                {
                    bufList.Add(new List<CharSetInfo>(new CharSetInfo[bufWidth * bufHeight]));
                }
            }


            if (input == '\n')
            {
                ++cursorPosition.y;
                cursorPosition.x = 0;
                return;
            }

            CharSetInfo temp = bufList[layer][GetBufferPos(cursorPosition.x, cursorPosition.y, layer)];

            temp.charInfo.Attributes = (short)(((int)background << 4) | ((int)foreground & 15));
            temp.charInfo.Char.UnicodeChar = input;
            temp.set = true;

            bufList[layer][GetBufferPos(cursorPosition.x, cursorPosition.y, layer)] = temp;

            if (cursorPosition.x == bufWidth - 1)
            {
                cursorPosition.x = 0;
                cursorPosition.y += 1;
            }
            else
                cursorPosition.x += 1;
        }
        public void AddToBuffer(int x, int y, int layer, char input, ConsoleColor foreground = ConsoleColor.White, ConsoleColor background = ConsoleColor.Black)
        {
            cursorPosition.x = x;
            cursorPosition.y = y;

            AddToBuffer(layer, input, foreground, background);

        }
        public void AddToBuffer(int x, int y, int layer, string input, ConsoleColor foreground = ConsoleColor.White, ConsoleColor background = ConsoleColor.Black)
        {
            cursorPosition.x = x;
            cursorPosition.y = y;

            for (int i = 0; i < input.Length; ++i)
            {
                AddToBuffer(layer, input[i], foreground, background);
            }
        }
        public void AddToBuffer(int layer, string input, ConsoleColor foreground = ConsoleColor.White, ConsoleColor background = ConsoleColor.Black)
        {
            AddToBuffer(cursorPosition.x, cursorPosition.y, layer, input, foreground, background);
        }


        public void DisplayBuffer()
        {
            CharInfo[] buffer = new CharInfo[bufWidth * bufHeight];

            for (int i = 0; i < bufList.Count; ++i)
            {
                for (int x = 0; x < bufList[i].Count; ++x)
                {
                    if (bufList[i][x].set)
                        buffer[x] = bufList[i][x].charInfo;
                }
            }

            WriteConsoleOutputW(handle, buffer, new Coord() { X = bufWidth, Y = bufHeight }, new Coord() { X = 0, Y = 0 }, ref rect);
        }

        public void ClearBuffer()
        {
            for (int i = 0; i < bufList.Count; ++i)
            {
                bufList[i].Clear();
                bufList[i].AddRange(new CharSetInfo[bufWidth * bufHeight]);
            }

            Console.Clear();
        }

        public void ClearLayer(int layer)
        {
            if (ValidLayer(layer))
            {
                bufList[layer].Clear();
                bufList[layer].AddRange(new CharSetInfo[bufWidth * bufHeight]);
            }
        }
    }
}
