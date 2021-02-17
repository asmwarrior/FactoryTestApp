testSequenceManager.addSequence("OLC Zhaga ECO");

ZhagaECO =
{
    testConnection: function ()
    {
        JlinkList.forEach(function(item){item.testConnection()});
    },

    downloadRailtest: function ()
    {
        let jlink = JlinkList[currentJLinkIndex];
        jlink.startScript("/sequences/OlcZhagaECO/olc_zhaga_railtest.jlink");
    },

    downloadSoftware: function ()
    {
        let jlink = JlinkList[currentJLinkIndex];
        jlink.startJLinkScript("/sequences/OlcZhagaECO/olc_zhaga_software.jlink");
    },

    openRailTestClient: function ()
    {
        let rail = railTestClientList[currentRailTestClientIndex];
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
        let rail = railTestClientList[currentRailTestClientIndex];
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
        let rail = railTestClientList[currentRailTestClientIndex];
        rail.syncCommand("dlpw", "1", 1000);
        rail.syncCommand("dali", "0xFE80 16 0 0", 1000);
        rail.syncCommand("dali", "0xFF90 16 0 1000000", 2000);
    },

    testRadio: function ()
    {
        let rail = railTestClientList[currentRailTestClientIndex];
        rail.testRadio();
    },

    testAccelerometer: function ()
    {
        let rail = railTestClientList[currentRailTestClientIndex];
        rail.testAccelerometer();
    },

    testLightSensor: function ()
    {
        let rail = railTestClientList[currentRailTestClientIndex];
        rail.testLightSensor();
    },

    testDALI: function ()
    {
        let rail = railTestClientList[currentRailTestClientIndex];
        rail.testDALI();
    },

    testGNSS: function ()
    {
        let rail = railTestClientList[currentRailTestClientIndex];
        rail.testGNSS();
    }
}

testSequenceManager.addTestFunction("Test connection to JLink", ZhagaECO.testConnection);
testSequenceManager.addTestFunction("Download Railtest", ZhagaECO.downloadRailtest);
testSequenceManager.addTestFunction("Open and check Railtest Client", ZhagaECO.openRailTestClient);
testSequenceManager.addTestFunction("Read unique device identifier (ID)", ZhagaECO.readChipID);
testSequenceManager.addTestFunction("Initialize Dali test", ZhagaECO.initDali);
testSequenceManager.addTestFunction("Test radio interface", ZhagaECO.testRadio);
testSequenceManager.addTestFunction("Test accelerometer", ZhagaECO.testAccelerometer);
testSequenceManager.addTestFunction("Test light sensor", ZhagaECO.testLightSensor);
testSequenceManager.addTestFunction("Test DALI", ZhagaECO.testDALI);
testSequenceManager.addTestFunction("Test GNSS", ZhagaECO.testGNSS);
testSequenceManager.addTestFunction("Download Software", ZhagaECO.downloadSoftware);
