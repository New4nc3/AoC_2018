class Program
{
    private const string INPUT = @"..\..\..\test.txt";
    //private const string INPUT = @"..\..\..\input.txt";
    private const int START_IDX = 5;
    private const int LAST_IDX = 36;

    class Node
    {
        public char Name { get; }
        public List<Node> NextSteps { get; private set; }

        public Node(char name)
        {
            Name = name;
            NextSteps = new List<Node>();
        }

        public void AddStep(Node node) =>
            NextSteps.Add(node);
    }

    static void Main()
    {
        var nodeMap = new Dictionary<char, Node>();
        var first = new HashSet<char>();
        var last = new HashSet<char>();

        using (var streamReader = new StreamReader(INPUT))
        {
            foreach (var entry in streamReader.ReadToEnd().Split("\r\n").Select(x => new { FIRST = x[START_IDX], LAST = x[LAST_IDX] }))
            {
                first.Add(entry.FIRST);
                last.Add(entry.LAST);

                Node lastNode;
                if (nodeMap.TryGetValue(entry.LAST, out Node? lastValue))
                    lastNode = lastValue;
                else
                {
                    lastNode = new Node(entry.LAST);
                    nodeMap.Add(entry.LAST, lastNode);
                }

                if (nodeMap.TryGetValue(entry.FIRST, out Node? firstValue))
                    firstValue.AddStep(lastNode);
                else
                {
                    var firstNode = new Node(entry.FIRST);
                    firstNode.AddStep(lastNode);
                    nodeMap.Add(firstNode.Name, firstNode);
                }
            }

            foreach (var lastItem in last)
                first.Remove(lastItem);
        }

        Console.WriteLine("Hello, World!");
    }
}
