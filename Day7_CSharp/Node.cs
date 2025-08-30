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
