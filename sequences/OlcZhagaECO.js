var SLOTS_NUMBER = 3
methodManager.addMethod("OLC Zhaga ECO");

ZhagaECO =
{
    measuringBoardIDs: ["5CDA73693935", "5CDA78693935", "5CDC87693935", "5CEE627B3935",  "5CDF60603935"],
    RfModuleId: "AL03RZIAA",

    //---

    openTestClients: function ()
    {
        GeneralCommands.openTestClients(ZhagaECO.measuringBoardIDs);
    },

    //---

    downloadRailtest: function ()
    {
        GeneralCommands.downloadRailtest("sequences/OLCZhagaECO/dummy_btl_efr32xg12.s37", "sequences/OLCZhagaECO/olc_zhaga_2l4l_railtest.hex");
    },

    //---

    detectDuts: function ()
    {
        actionHintWidget.showProgressHint("Detecting DUTs in the testing fixture...");

        for (var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (var i = 0; i < testClientList.length; i++)
            {
                let testClient = testClientList[i];

                if(!testClient.isConnected())
                    continue;

                testClient.powerOn(slot);
            }
        }
        delay(1000);

        for (slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (i = 0; i < testClientList.length; i++)
            {
                var testClient = testClientList[i];

                if(!testClient.isConnected())
                    continue;

                testClient.setDutProperty(slot, "state", 0);
                testClient.setDutProperty(slot, "checked", false);
                testClientList[i].setDutProperty(slot, "voltageChecked", false);
                for (j = 0; j < 3; j++)
                    if (testClient.readAIN(slot, 1, 0) > 69000)
                    {
                        logger.logSuccess("Device connected to the slot " + slot + " of the test board " + testClient.no() + " detected.");
                        logger.logDebug("Device connected to the slot " + slot + " of the test board " + testClient.no() + " detected.");
                        testClient.setDutProperty(slot, "state", 1);
                        testClient.setDutProperty(slot, "checked", true);
                        testClientList[i].setDutProperty(slot, "voltageChecked", true);
                        break;
                    }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    //---

    downloadSoftware: function ()
    {
        GeneralCommands.downloadSoftware("sequences/OlcZhagaECO/olc_zhaga_software.hex");
    },

    //---

    powerTable: [   -90, -90, -90,
                    -90, -90, -90,
                    -90, -90, -90,
                    -60, -90, -90,
                    -60, -90, -90  ],

    testRadio: function ()
    {
        GeneralCommands.testRadio(ZhagaECO.RfModuleId, 19, ZhagaECO.powerTable, -90, 0, 50);
    },

    //---

    startTesting: function ()
    {
        GeneralCommands.testConnection();
        ZhagaECO.openTestClients();

        if(!GeneralCommands.isMethodCorrect)
            return;

        GeneralCommands.clearDutsInfo();
        ZhagaECO.detectDuts();
        GeneralCommands.unlockAndEraseChip();
        ZhagaECO.downloadRailtest();
        GeneralCommands.readChipId();
        GeneralCommands.testDALI();
        GeneralCommands.testAccelerometer();
        GeneralCommands.testLightSensor();
        ZhagaECO.testRadio();
        ZhagaECO.checkTestingCompletion();
        ZhagaECO.downloadSoftware();
        GeneralCommands.powerOff();
    },

    //---

    checkTestingCompletion: function ()
    {
        for(var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (var i = 0; i < testClientList.length; i++)
            {
                let testClient = testClientList[i];
                if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
                {
                    if (testClient.dutProperty(slot, "id") !== ""
                        && testClient.dutProperty(slot, "voltageChecked")
                        && testClient.dutProperty(slot, "lightSensChecked")
                        && testClient.dutProperty(slot, "daliChecked")
                        && testClient.dutProperty(slot, "radioChecked")
                        && testClient.dutProperty(slot, "accelChecked"))
                    {
                        testClient.setDutProperty(slot, "state", 2);
                    }
                    else
                    {
                        testClient.setDutProperty(slot, "state", 3);
                    }
                }
                else
                    if(testClient.isDutAvailable(slot) && !testClient.dutProperty(slot, "railtestDownloaded"))
                    {
                        testClient.setDutProperty(slot, "checked", true);
                        testClient.setDutProperty(slot, "state", 3);
                    }
            }
        }
    }
}

methodManager.addFunctionToGeneralList("Full cycle testing", ZhagaECO.startTesting);
methodManager.addFunctionToGeneralList("Test connection to JLink", GeneralCommands.testConnection);
methodManager.addFunctionToGeneralList("Establish connection to sockets", ZhagaECO.openTestClients);
methodManager.addFunctionToGeneralList("Clear previous test results for DUTs", GeneralCommands.clearDutsInfo);
methodManager.addFunctionToGeneralList("Detect DUTs", ZhagaECO.detectDuts);
methodManager.addFunctionToGeneralList("Unlock and erase chip", GeneralCommands.unlockAndEraseChip);
methodManager.addFunctionToGeneralList("Download Railtest", ZhagaECO.downloadRailtest);
methodManager.addFunctionToGeneralList("Read CSA", GeneralCommands.readCSA);
methodManager.addFunctionToGeneralList("Read Temperature", GeneralCommands.readTemperature);
methodManager.addFunctionToGeneralList("Supply power to DUTs", GeneralCommands.powerOn);
methodManager.addFunctionToGeneralList("Power off DUTs", GeneralCommands.powerOff);
methodManager.addFunctionToGeneralList("Read unique device identifiers (ID)", GeneralCommands.readChipId);
methodManager.addFunctionToGeneralList("Test accelerometer", GeneralCommands.testAccelerometer);
methodManager.addFunctionToGeneralList("Test light sensor", GeneralCommands.testLightSensor);
methodManager.addFunctionToGeneralList("Test radio interface", ZhagaECO.testRadio);
methodManager.addFunctionToGeneralList("Test DALI", GeneralCommands.testDALI);
methodManager.addFunctionToGeneralList("Check Testing Completion", ZhagaECO.checkTestingCompletion);
methodManager.addFunctionToGeneralList("Download Software", ZhagaECO.downloadSoftware);
