class NodeProcessor
{
    private string _filename;
    private HashSet<char> _roots;
    private Dictionary<char, Node> _nodeMap;

    public NodeProcessor(string filename)
    {
        _filename = filename;
        _roots = new HashSet<char>();
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
    {
        var candidates = _roots.ToList();
        candidates.Sort((x, y) => x.CompareTo(y));

        var startNode = _nodeMap[candidates.First()];
        var queue = candidates
            .Skip(1)
            .Select(x => _nodeMap[x]);

        Console.Write("Part 1. ");
        IterateRecursive(startNode, queue);
        Console.WriteLine();
    }

    private void IterateRecursive(Node start, IEnumerable<Node> queue)
    {
        Console.Write(start.Name);

        if (start.NextSteps.Count == 0 && !queue.Any())
            return;

        var candidates = start.NextSteps
            .Concat(queue)
            .Distinct()
            .OrderBy(x => x.NextSteps.Count == 0 ? 1 : 0)
            .ThenBy(x => x.Name)
            .ToList();

        IterateRecursive(candidates.First(), candidates.Skip(1));
    }
}
