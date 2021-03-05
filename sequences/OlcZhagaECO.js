methodManager.addMethod("OLC Zhaga ECO");

ZhagaECO =
{
    downloadRailtest: function ()
    {  
        GeneralCommands.startJlinkScript("sequences/OLCZhagaECO/download_railtest.jlink");
    },

    //---

    downloadSoftware: function ()
    {
        GeneralCommands.startJlinkScript("sequences/OLCZhagaECO/download_software.jlink");
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

        GeneralCommands.detectDuts();
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

methodManager.addFunctionToGeneralList("Full cycle testing", ZhagaECO.startTesting);
methodManager.addFunctionToGeneralList("Test connection to JLink", GeneralCommands.testConnection);
methodManager.addFunctionToGeneralList("Download Railtest", ZhagaECO.downloadRailtest);
methodManager.addFunctionToGeneralList("Read CSA", GeneralCommands.readCSA);
methodManager.addFunctionToGeneralList("Detect DUTs", GeneralCommands.detectDuts);
methodManager.addFunctionToGeneralList("Supply power to DUTs", GeneralCommands.powerOn);
methodManager.addFunctionToGeneralList("Power off DUTs", GeneralCommands.powerOff);
methodManager.addFunctionToGeneralList("Read unique device identifiers (ID)", GeneralCommands.readChipId);
methodManager.addFunctionToGeneralList("Check voltage on AIN 1 (3.3V)", ZhagaECO.checkAinVoltage);
methodManager.addFunctionToGeneralList("Test accelerometer", GeneralCommands.testAccelerometer);
methodManager.addFunctionToGeneralList("Test light sensor", GeneralCommands.testLightSensor);
methodManager.addFunctionToGeneralList("Test DALI", GeneralCommands.testDALI);
methodManager.addFunctionToGeneralList("Check Testing Completion", ZhagaECO.checkTestingCompletion);

//testSequenceManager.addFunctionToGeneralList("Test radio interface", ZhagaECO.testRadio);
//testSequenceManager.addFunctionToGeneralList("Test GNSS", ZhagaECO.testGNSS);
//testSequenceManager.addFunctionToGeneralList("Download Software", ZhagaECO.downloadSoftware);

