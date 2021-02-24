testSequenceManager.addMethod("OLC Zhaga ECO");

ZhagaECO =
{
    testCommand: function ()
    {
        //testClientList[0].readCSA(0);
        testClientList.forEach(function(item){item.readCSA(0)});
    },

    testConnection: function ()
    {
        JlinkList.forEach(function(item){item.testConnection()});
    },

    downloadRailtest: function ()
    {
        let jlink = JlinkList[currentJLinkIndex];
        jlink.startJlinkCommands([
                                    "eoe 1",
                                    "swdselect",
                                    "si swd",
                                    "speed 5000",
                                    "Device = EFR32FG12PXXXF1024",
                                    "connect",
                                    "erase",
                                    "loadfile dummy_btl_efr32xg12.s37",
                                    "loadfile olc_zhaga_railtest.hex",
                                    "r"
                                 ]);
    },

    downloadSoftware: function ()
    {
        let jlink = JlinkList[currentJLinkIndex];
        jlink.startJLinkScript("/sequences/OlcZhagaECO/olc_zhaga_software.jlink");
    },

    openRailTestClient: function ()
    {
        railTestClientList.forEach(function(item)
        {
            var rail = item;
            if (!rail.open())
            {
                logger.logError("Cannot open RAILTEST serial port!");
            }

            if (!rail.waitCommandPrompt())
            {
                logger.logError("Timeout waiting RAILTEST command prompt!");
            }
        });

    },

    readChipID: function ()
    {
        let rail = railTestClientList[currentRailTestClientIndex];
        rail.readChipId();
        let id = rail.currentChipId();

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

testSequenceManager.addFunctionToGeneralList("testCommand", ZhagaECO.testCommand);
testSequenceManager.addFunctionToGeneralList("Test connection to JLink", ZhagaECO.testConnection);
testSequenceManager.addFunctionToGeneralList("Download Railtest", ZhagaECO.downloadRailtest);
testSequenceManager.addFunctionToGeneralList("Open and check Railtest Client", ZhagaECO.openRailTestClient);
testSequenceManager.addFunctionToGeneralList("Read unique device identifier (ID)", ZhagaECO.readChipID);
testSequenceManager.addFunctionToGeneralList("Initialize Dali test", ZhagaECO.initDali);
testSequenceManager.addFunctionToGeneralList("Test radio interface", ZhagaECO.testRadio);
testSequenceManager.addFunctionToGeneralList("Test accelerometer", ZhagaECO.testAccelerometer);
testSequenceManager.addFunctionToGeneralList("Test light sensor", ZhagaECO.testLightSensor);
testSequenceManager.addFunctionToGeneralList("Test DALI", ZhagaECO.testDALI);
testSequenceManager.addFunctionToGeneralList("Test GNSS", ZhagaECO.testGNSS);
testSequenceManager.addFunctionToGeneralList("Download Software", ZhagaECO.downloadSoftware);

testSequenceManager.addFunctionToTestSequence("Download Railtest", ZhagaECO.downloadRailtest);
testSequenceManager.addFunctionToTestSequence("Open and check Railtest Client", ZhagaECO.openRailTestClient);
testSequenceManager.addFunctionToTestSequence("Read unique device identifier (ID)", ZhagaECO.readChipID);
testSequenceManager.addFunctionToTestSequence("Initialize Dali test", ZhagaECO.initDali);
testSequenceManager.addFunctionToTestSequence("Test radio interface", ZhagaECO.testRadio);
testSequenceManager.addFunctionToTestSequence("Test accelerometer", ZhagaECO.testAccelerometer);
testSequenceManager.addFunctionToTestSequence("Test light sensor", ZhagaECO.testLightSensor);
testSequenceManager.addFunctionToTestSequence("Test DALI", ZhagaECO.testDALI);
testSequenceManager.addFunctionToTestSequence("Test GNSS", ZhagaECO.testGNSS);
testSequenceManager.addFunctionToTestSequence("Download Software", ZhagaECO.downloadSoftware);
