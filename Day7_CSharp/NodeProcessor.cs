class NodeProcessor
{
    private string _filename;
    private Dictionary<char, Node> _nodeMap;

    public NodeProcessor(string filename)
    {
        _filename = filename;
        _nodeMap = new Dictionary<char, Node>();

        Initialize();
    }

    private void Initialize()
    {
        const int START_IDX = 5;
        const int LAST_IDX = 36;
        const string END_LINE = "\r\n";

        string[] data;
        using (var streamReader = new StreamReader(_filename))
            data = streamReader.ReadToEnd().Split(END_LINE);

        foreach (var entry in data)
        {
            var firstEntry = entry[START_IDX];
            var lastEntry = entry[LAST_IDX];

            Node lastNode;
            if (_nodeMap.TryGetValue(lastEntry, out Node? lastValue))
                lastNode = lastValue;
            else
            {
                lastNode = new Node(lastEntry);
                _nodeMap.Add(lastEntry, lastNode);
            }

            if (_nodeMap.TryGetValue(firstEntry, out Node? firstValue))
                lastNode.AddRequirement(firstValue);
            else
            {
                var firstNode = new Node(firstEntry);
                lastNode.AddRequirement(firstNode);
                _nodeMap.Add(firstNode.Name, firstNode);
            }
        }
    }

    public void SolvePart1()
    {
        Console.Write("Part 1. ");

        while (_nodeMap.Count > 0)
        {
            var nodeToLaunch = _nodeMap.Values
                .Where(x => !x.Launched && x.MeetsRequirements)
                .OrderBy(x => x.Name)
                .First();

            nodeToLaunch.Launch();
            _nodeMap.Remove(nodeToLaunch.Name);
        }

        Console.WriteLine();
    }
}
