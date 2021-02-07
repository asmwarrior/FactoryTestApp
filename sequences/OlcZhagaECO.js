testSequenceManager.addSequence("OLC Zhaga ECO");

testSequenceManager.addTestFunction("test Zhaga1", testZhaga1);
testSequenceManager.addTestFunction("test Zhaga2", testZhaga2);
testSequenceManager.addTestFunction("test Zhaga3", testZhaga3);

function testZhaga1()
{
    console.log("test Nema called");
}

function testZhaga2()
{
    console.log("test Nema2 called");
}

function testZhaga3()
{
    console.log("test Nema3 called");
}
