testSequenceManager.addSequence("OLC Nema");

testSequenceManager.addTestFunction("test Nema1", testNema1);
testSequenceManager.addTestFunction("test Nema2", testNema2);
testSequenceManager.addTestFunction("test Nema3", testNema3);

function testNema1()
{
    logger.logInfo("test Nema1 called");
}

function testNema2()
{
    console.log("test Nema2 called");
}

function testNema3()
{
    console.log("test Nema3 called");
}
