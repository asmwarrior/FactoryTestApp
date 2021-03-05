methodManager.addMethod("OLC Zhaga ECO");

ZhagaECO =
{
    downloadRailtest: function ()
    {  
        GeneralCommands.downloadRailtest("sequences/OLCZhagaECO/download_railtest.jlink");
//        for (let slot = 1; slot < 4; slot++)
//        {
//            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
//            {
//                testClient.switchSWD(slot);
//                testClient.delay(100);
//                jlink.startScript("sequences/OLCZhagaECO/download_railtest.jlink");
//            }
//        }
    },

    downloadSoftware: function ()
    {
        for(let i = 0; i < testClientList.length; i++)
        {
            testClient = testClientList[i];
            for (let slot = 1; slot < 4; slot++)
            {
                if(testClient.dutState(slot) === 2)
                {
                    testClient.switchSWD(slot);
                    testClient.delay(500);
                    JlinkList[i].startScript("sequences/OLCZhagaECO/download_software.jlink");
                }
            }
        }
    },

    readChipID: function ()
    {
        testClientList.forEach(
        function(item)
        {
            for(var slot = 1; slot < 4; slot++)
            {
                if(item.isDutAvailable(slot) && item.isDutChecked(slot))
                {
                    item.switchSWD(slot);
                    item.readChipId(slot);
                }
            }

//            mainWindow.delay(100);
        });
    },

    checkAinVoltage: function ()
    {
        testClientList.forEach(
        function(item)
        {
            for(var slot = 1; slot < 4; slot++)
            {
                if(item.isDutAvailable(slot) && item.isDutChecked(slot))
                {
                    item.readAIN(slot, 1, 0);
                }
            }

            mainWindow.delay(100);
        });
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
        testClientList.forEach(
        function(item)
        {
            for(var slot = 1; slot < 4; slot++)
            {
                if(item.isDutAvailable(slot) && item.isDutChecked(slot))
                {
                    item.testAccelerometer(slot);
                }
            }

            mainWindow.delay(100);
        });
    },

    testLightSensor: function ()
    {
        testClientList.forEach(
        function(item)
        {
            for(var slot = 1; slot < 4; slot++)
            {
                if(item.isDutAvailable(slot) && item.isDutChecked(slot))
                {
                    item.testLightSensor(slot);
                }
            }

            mainWindow.delay(100);
        });
    },

    testDALI: function ()
    {
        testClientList.forEach(
        function(item)
        {
            for(var slot = 1; slot < 4; slot++)
            {
                if(item.isDutAvailable(slot) && item.isDutChecked(slot))
                {
                    item.switchSWD(slot);
                    item.delay(500);
                    item.daliOn();
                    item.delay(2000);

                    item.testDALI();
                    item.delay(500);

                    item.daliOff();
                    item.delay(500);
                }
            }

            mainWindow.delay(100);
        });
    },

    testGNSS: function ()
    {
        let rail = railTestClientList[currentRailTestClientIndex];
        rail.testGNSS();
    },

    checkTestingCompletion: function ()
    {
        testClientList.forEach(
        function(item)
        {
            item.checkTestingCompletion();
        });
    }
}

methodManager.addFunctionToGeneralList("Test connection to JLink", GeneralCommands.testConnection);
methodManager.addFunctionToGeneralList("Download Railtest", ZhagaECO.downloadRailtest);
methodManager.addFunctionToGeneralList("Read CSA", GeneralCommands.readCSA);
methodManager.addFunctionToGeneralList("Detect DUTs", GeneralCommands.detectDuts);
methodManager.addFunctionToGeneralList("Supply power to DUTs", GeneralCommands.powerOn);
methodManager.addFunctionToGeneralList("Power off DUTs", GeneralCommands.powerOff);
methodManager.addFunctionToGeneralList("Read unique device identifiers (ID)", GeneralCommands.readChipId);

//testSequenceManager.addFunctionToGeneralList("Test connection to JLink", ZhagaECO.testConnection);
//testSequenceManager.addFunctionToGeneralList("Check voltage on AIN 1 (3.3V)", ZhagaECO.checkAinVoltage);
//testSequenceManager.addFunctionToGeneralList("Initialize Dali test", ZhagaECO.initDali);
//testSequenceManager.addFunctionToGeneralList("Test radio interface", ZhagaECO.testRadio);
//testSequenceManager.addFunctionToGeneralList("Test accelerometer", ZhagaECO.testAccelerometer);
//testSequenceManager.addFunctionToGeneralList("Test light sensor", ZhagaECO.testLightSensor);
//testSequenceManager.addFunctionToGeneralList("Test DALI", ZhagaECO.testDALI);
//testSequenceManager.addFunctionToGeneralList("Test GNSS", ZhagaECO.testGNSS);
//testSequenceManager.addFunctionToGeneralList("Download Software", ZhagaECO.downloadSoftware);
//testSequenceManager.addFunctionToGeneralList("Check Testing Completion", ZhagaECO.checkTestingCompletion);

//testSequenceManager.addFunctionToTestSequence("Download Railtest", ZhagaECO.downloadRailtest);
//testSequenceManager.addFunctionToTestSequence("Read unique device identifiers (ID)", ZhagaECO.readChipID);
//testSequenceManager.addFunctionToTestSequence("Check voltage on AIN 1 (3.3V)", ZhagaECO.checkAinVoltage);
//testSequenceManager.addFunctionToTestSequence("Initialize Dali test", ZhagaECO.initDali);
//testSequenceManager.addFunctionToTestSequence("Test radio interface", ZhagaECO.testRadio);
//testSequenceManager.addFunctionToTestSequence("Test accelerometer", ZhagaECO.testAccelerometer);
//testSequenceManager.addFunctionToTestSequence("Test light sensor", ZhagaECO.testLightSensor);
//testSequenceManager.addFunctionToTestSequence("Test DALI", ZhagaECO.testDALI);
//testSequenceManager.addFunctionToTestSequence("Test GNSS", ZhagaECO.testGNSS);
//testSequenceManager.addFunctionToTestSequence("Download Software", ZhagaECO.downloadSoftware);
//testSequenceManager.addFunctionToTestSequence("Check Testing Completion", ZhagaECO.checkTestingCompletion);
