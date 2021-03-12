methodManager.addMethod("OLC Zhaga ECO");

ZhagaECO =
{
    downloadRailtest: function ()
    {
        testClient.commandSequenceStarted();

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
//                if(jlink.erase() < 0)
//                {
//                    return;
//                }

//                jlink.downloadFile("sequences/OLCZhagaECO/dummy_btl_efr32xg12.s37", 0);
//                jlink.downloadFile("sequences/OLCZhagaECO/olc_zhaga_2l4l_railtest.hex", 0);
//                jlink.reset();
//                jlink.go();
                jlink.close();
            }
        }

        testClient.commandSequenceFinished();
    },

    //---

    downloadSoftware: function ()
    {
        testClient.commandSequenceStarted();

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
//                if(jlink.erase() < 0)
//                {
//                    return;
//                }

//                jlink.downloadFile("sequences/OLCZhagaECO/olc_zhaga_software.hex", 0);
                jlink.close();
            }
        }

        testClient.commandSequenceFinished();
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

        testClient.commandSequenceFinished();
    },

    //---

    checkTestingCompletion: function ()
    {
        for(var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                if( testClient.getDutProperty(slot, "id") !== "" &&
                    testClient.getDutProperty(slot, "voltageChecked") &&
                    testClient.getDutProperty(slot, "lightSensChecked") &&
                    testClient.getDutProperty(slot, "daliChecked") &&
                    //testClient.getDutProperty(slot, "radioChecked") &&
                    testClient.getDutProperty(slot, "accelChecked")
                   )
                {
                    testClient.on_setDutProperty(slot, "state", 2);
                }

                else
                {
                    testClient.on_setDutProperty(slot, "state", 3);
                }

                testClient.slotFullyTested(slot);
            }
        }
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
methodManager.addFunctionToGeneralList("Test radio interface", GeneralCommands.testRadio, true);
methodManager.addFunctionToGeneralList("Download software", ZhagaECO.downloadSoftware, true);
methodManager.addFunctionToGeneralList("Check Testing Completion", ZhagaECO.checkTestingCompletion);
