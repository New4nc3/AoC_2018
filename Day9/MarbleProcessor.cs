class MarbleProcessor
{
    public int PlayersCount { get; }
    public int LastMarbleValue { get; private set; }

    public LinkedList<ulong> Marbles { get; }
    public Dictionary<int, ulong> Players { get; }

    public MarbleProcessor(int players, int lastMarbleValue)
    {
        (PlayersCount, LastMarbleValue) = (players, lastMarbleValue);

        Marbles = new LinkedList<ulong>();
        Players = new Dictionary<int, ulong>();
    }

    public void Solve(bool part2)
    {
        InitMarbles();
        InitPlayers();

        if (part2)
            LastMarbleValue *= 100;

        var currentMarble = Marbles.First;
        int currentPlayerIndex = 0;
        int marbleValue = 1;

        while (marbleValue <= LastMarbleValue)
        {
            if (marbleValue % 23 == 0)
            {
                Players[currentPlayerIndex] += (ulong)marbleValue;

                for (var i = 0; i < 7; ++i)
                {
                    currentMarble = currentMarble!.Previous;
                    if (currentMarble == null)
                        currentMarble = Marbles.Last;
                }

                var valueToReference = currentMarble!.Next;
                if (valueToReference == null)
                    valueToReference = Marbles.First;

                Players[currentPlayerIndex] += currentMarble.Value;
                Marbles.Remove(currentMarble);
                currentMarble = valueToReference;
            }
            else
            {
                currentMarble = currentMarble!.Next;
                if (currentMarble == null)
                    currentMarble = Marbles.First;

                Marbles.AddAfter(currentMarble!, (ulong)marbleValue);
                currentMarble = currentMarble!.Next;
            }

            ++currentPlayerIndex;
            if (currentPlayerIndex >= PlayersCount)
                currentPlayerIndex = 0;

            ++marbleValue;
        }

        Console.WriteLine($"Part {(part2 ? "2" : "1")}. Max score: {Players.Values.Max()}");
    }

    private void InitMarbles()
    {
        Marbles.Clear();
        Marbles.AddFirst(0);
    }

    private void InitPlayers()
    {
        Players.Clear();

        for (var i = 0; i < PlayersCount; ++i)
            Players.Add(i, 0);
    }
}
