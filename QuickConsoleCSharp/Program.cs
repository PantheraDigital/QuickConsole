using System;

namespace QuickConsoleCSharp
{
    class Program
    {
        static void Main(string[] args)
        {
            QuickConsole quickConsole = QuickConsole.GetInstance();

            quickConsole.AddToBuffer(0, "Hello World");
            quickConsole.DisplayBuffer();
            Console.ReadKey();

            quickConsole.SetCursorPosition(new Vector2(6, 0));
            quickConsole.AddToBuffer(1, "world!", ConsoleColor.Red);
            quickConsole.DisplayBuffer();
            Console.ReadKey();

            quickConsole.ClearLayer(1);
            quickConsole.DisplayBuffer();
            Console.SetCursorPosition(11, 0);
            Console.ReadKey();
        }
    }
}
