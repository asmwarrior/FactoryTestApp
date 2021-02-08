testSequenceManager.addSequence("OLC Zhaga ECO");

ZhagaECO =
{
    downloadRailtest: function ()
    {
        jlink.startJLinkScript("olc_zhaga_railtest.jlink");
    }
}

testSequenceManager.addTestFunction("Download Railtest", ZhagaECO.downloadRailtest);
