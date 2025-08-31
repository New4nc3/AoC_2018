class Node
{
    public bool Launched { get; private set; }
    public bool ReservedByWorker { get; private set; }
    public bool MeetsRequirements => Requirements.Count == 0 || Requirements.All(x => x.Launched);

    public char Name { get; }
    public List<Node> Requirements { get; private set; }

    public Node(char name)
    {
        Name = name;
        Requirements = new List<Node>();
    }

    public void AddRequirement(Node node) =>
        Requirements.Add(node);

    public void Launch()
    {
        Console.Write(Name);
        Launched = true;
    }

    public void SetAsReservedByWorker() =>
        ReservedByWorker = true;

    public void ReleaseAsReservedByWorker() =>
        ReservedByWorker = false;

    public override string ToString() =>
        $"{Name} {MeetsRequirements} {Launched}";
}
