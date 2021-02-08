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

    openRailTestClient: function ()
    {
        if (!rail.open())
        {
            logger.logError("Cannot open RAILTEST serial port!");
        }

        if (!rail.waitCommandPrompt())
        {
            logger.logError("Timeout waiting RAILTEST command prompt!");
        }
    },

    readChipID: function ()
    {
        let id = rail.readChipId();

        if(id == "")
        {
            logger.logError("Cannot read unique device identifier!");
        }

        else
        {
            logger.logSuccess("Device ID: " + id);
        }
    },

    initDali: function ()
    {
        rail.syncCommand("dlpw", "1", 1000);
        rail.syncCommand("dali", "0xFE80 16 0 0", 1000);
        rail.syncCommand("dali", "0xFF90 16 0 1000000", 2000);
    },

    testRadio: function ()
    {
        rail.testRadio();
    },

    testAccelerometer: function ()
    {
        rail.testAccelerometer();
    }
}

testSequenceManager.addTestFunction("Download Railtest", ZhagaECO.downloadRailtest);
testSequenceManager.addTestFunction("Open and check Railtest Client", ZhagaECO.openRailTestClient);
testSequenceManager.addTestFunction("Read unique device identifier (ID)", ZhagaECO.readChipID);
testSequenceManager.addTestFunction("Initialize Dali test", ZhagaECO.initDali);
testSequenceManager.addTestFunction("Test radio interface", ZhagaECO.testRadio);
testSequenceManager.addTestFunction("Test accelerometer", ZhagaECO.testAccelerometer);
testSequenceManager.addTestFunction("Download Software", ZhagaECO.downloadSoftware);
