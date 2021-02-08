testSequenceManager.addSequence("OLC Zhaga ECO");

ZhagaECO =
{
    downloadRailtest: function ()
    {
        jlink.startJLinkScript("/sequences/OlcZhagaECO/olc_zhaga_railtest.jlink");
    },

    downloadSoftware: function ()
    {
        jlink.startJLinkScript("/sequences/OlcZhagaECO/olc_zhaga_software.jlink");
    },

    checkRailTestClient: function ()
    {

    }
}

testSequenceManager.addTestFunction("Download Railtest", ZhagaECO.downloadRailtest);
testSequenceManager.addTestFunction("Download Software", ZhagaECO.downloadSoftware);
