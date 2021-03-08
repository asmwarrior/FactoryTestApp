methodManager.addMethod("OLC Zhaga ECO");

ZhagaECO =
{
    downloadRailtest: function ()
    {
        for(var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                testClient.powerOn(slot);
                testClient.delay(1000);
                testClient.switchSWD(slot);

                jlink.selectByUSB();
                jlink.open();
                jlink.setDevice("EFR32FG12PXXXF1024");
                jlink.select();
                jlink.setSpeed(5000);
                jlink.connect();
                jlink.erase();
                jlink.downloadFile("sequences/OLCZhagaECO/dummy_btl_efr32xg12.s37", 0);
                jlink.downloadFile("sequences/OLCZhagaECO/olc_zhaga_2l4l_railtest.hex", 0);
                jlink.reset();
                jlink.go();
                jlink.close();
            }
        }
    },

    //---

    downloadSoftware: function ()
    {
        GeneralCommands.startJlinkScript("sequences/OLCZhagaECO/download_software.jlink");
    },

    testRadio: function ()
    {
        for(var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                console.log("Testing radio interface for DUT " + testClient.dutNo(slot));
            }
        }
    },

    //---

    checkAinVoltage: function ()
    {
        for(var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                testClient.readAIN(slot, 1, 0);
            }

            testClient.delay(100);
        }
    },

    //---

    startTesting: function ()
    {
        testClient.commandSequenceStarted();

        if(!testClient.isActive())
        {
            testClient.commandSequenceFinished();
            return;
        }

        GeneralCommands.powerOn();
        testClient.delay(1000);
        GeneralCommands.readChipId();
        ZhagaECO.checkAinVoltage();
        GeneralCommands.testAccelerometer();
        GeneralCommands.testLightSensor();
        GeneralCommands.testDALI();
        ZhagaECO.checkTestingCompletion();

        testClient.commandSequenceFinished();
    },

    //---

    checkTestingCompletion: function ()
    {
        testClient.checkTestingCompletion();
    }
}

methodManager.addFunctionToGeneralList("Detect DUTs", GeneralCommands.detectDuts);
methodManager.addFunctionToGeneralList("Download Railtest", ZhagaECO.downloadRailtest, true);
methodManager.addFunctionToGeneralList("Full cycle testing", ZhagaECO.startTesting);
methodManager.addFunctionToGeneralList("Test connection to JLink", GeneralCommands.testConnection);
methodManager.addFunctionToGeneralList("Read CSA", GeneralCommands.readCSA);
methodManager.addFunctionToGeneralList("Supply power to DUTs", GeneralCommands.powerOn);
methodManager.addFunctionToGeneralList("Power off DUTs", GeneralCommands.powerOff);
methodManager.addFunctionToGeneralList("Read unique device identifiers (ID)", GeneralCommands.readChipId);
methodManager.addFunctionToGeneralList("Check voltage on AIN 1 (3.3V)", ZhagaECO.checkAinVoltage);
methodManager.addFunctionToGeneralList("Test accelerometer", GeneralCommands.testAccelerometer);
methodManager.addFunctionToGeneralList("Test light sensor", GeneralCommands.testLightSensor);
methodManager.addFunctionToGeneralList("Test DALI", GeneralCommands.testDALI);
methodManager.addFunctionToGeneralList("Test radio interface", ZhagaECO.testRadio, true);
methodManager.addFunctionToGeneralList("Check Testing Completion", ZhagaECO.checkTestingCompletion);

//testSequenceManager.addFunctionToGeneralList("Test GNSS", ZhagaECO.testGNSS);
//testSequenceManager.addFunctionToGeneralList("Download Software", ZhagaECO.downloadSoftware);

