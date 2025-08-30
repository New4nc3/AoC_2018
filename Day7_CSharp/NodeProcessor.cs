class NodeProcessor
{
    private string _filename;
    private Dictionary<char, Node> _nodeMap;
    private HashSet<char> _roots;

    public NodeProcessor(string filename)
    {
        _filename = filename;
        _nodeMap = new Dictionary<char, Node>();
        _roots = new HashSet<char>();

        Initialize();
    }

    private void Initialize()
    {
        const int START_IDX = 5;
        const int LAST_IDX = 36;

        string[] data;
        using (var streamReader = new StreamReader(_filename))
            data = streamReader.ReadToEnd().Split("\r\n");

        var last = new HashSet<char>();
        foreach (var entry in data)
        {
            var firstEntry = entry[START_IDX];
            var lastEntry = entry[LAST_IDX];

            _roots.Add(firstEntry);
            last.Add(lastEntry);

            Node lastNode;
            if (_nodeMap.TryGetValue(lastEntry, out Node? lastValue))
                lastNode = lastValue;
            else
            {
                lastNode = new Node(lastEntry);
                _nodeMap.Add(lastEntry, lastNode);
            }

            if (_nodeMap.TryGetValue(firstEntry, out Node? firstValue))
                firstValue.AddStep(lastNode);
            else
            {
                var firstNode = new Node(firstEntry);
                firstNode.AddStep(lastNode);
                _nodeMap.Add(firstNode.Name, firstNode);
            }
        }

        foreach (var lastItem in last)
            _roots.Remove(lastItem);
    }

    public void SolvePart1()
    { }
}
