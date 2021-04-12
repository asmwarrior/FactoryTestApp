var SLOTS_NUMBER = 3
methodManager.addMethod("OLC Zhaga STD");

ZhagaSTD =
{
    measuringBoardIDs: ["5CDA73693935", "5CDA78693935", "5CDC87693935", "5CEE627B3935",  "5CDF60603935"],
    RfModuleId: "AL03RZIAA",

    //---

    openTestClients: function ()
    {
        GeneralCommands.openTestClients(ZhagaSTD.measuringBoardIDs);
    },

    //---

    downloadRailtest: function ()
    {
        for (var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (var i = 0; i < testClientList.length; i++)
            {
                let testClient = testClientList[i];
                let jlink = jlinkList[i];
                if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
                {
                    testClient.powerOn(slot);
                    delay(1000);
                    testClient.switchSWD(slot);

                    jlink.selectByUSB();
                    jlink.open();
                    jlink.setDevice("EFR32FG12PXXXF1024");
                    jlink.select();
                    jlink.setSpeed(5000);
                    jlink.connect();

                    let error = jlink.erase();
                    if(error < 0)
                    {
                        logger.logError("Unable to earase chip flash memory in DUT " + testClient.dutNo(slot));
                        logger.logDebug("An error occured when erasing chip in DUT " + testClient.dutNo(slot) + " Error code: " + error);
                    }

                    else
                    {
                        logger.logInfo("Chip flash in DUT " + testClient.dutNo(slot) + " has been erased.");
                    }

                    jlink.downloadFile("sequences/OLCZhagaECO/dummy_btl_efr32xg12.s37", 0);
                    error = jlink.downloadFile("sequences/OLCZhagaECO/olc_zhaga_2l4l_railtest.hex", 0);

                    if(error < 0)
                    {
                        logger.logError("Failed to load the Railtest into the chip flash memory for DUT " + testClient.dutNo(slot));
                        logger.logDebug("An error occured when downloading olc_zhaga_2l4l_railtest.hex for DUT " + testClient.dutNo(slot) + " Error code: " + error);
                    }

                    else
                    {
                        logger.logInfo("Railtest firmware has been downloaded in DUT " + testClient.dutNo(slot));
                    }

                    jlink.reset();
                    jlink.go();
                    jlink.close();
                }
            }
        }
    },

    //---

    downloadSoftware: function ()
    {
        for (var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (var i = 0; i < testClientList.length; i++)
            {
                let testClient = testClientList[i];
                let jlink = jlinkList[i];
                if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot) && (testClient.dutState(slot) === 2))
                {
                    testClient.powerOn(slot);
                    delay(1000);
                    testClient.switchSWD(slot);

                    jlink.selectByUSB();
                    jlink.open();
                    jlink.setDevice("EFR32FG12PXXXF1024");
                    jlink.select();
                    jlink.setSpeed(5000);
                    jlink.connect();

                    let error = jlink.erase();
                    if(error < 0)
                    {
                        logger.logError("Unable to earase chip flash memory in DUT " + testClient.dutNo(slot));
                        logger.logDebug("An error occured when erasing chip in DUT " + testClient.dutNo(slot) + " Error code: " + error);
                    }

                    else
                    {
                        logger.logInfo("Chip flash in DUT " + testClient.dutNo(slot) + " has been erased.");
                    }

                    error = jlink.downloadFile("sequences/OlcZhagaSTD/ZhagaSTD_software.hex", 0);

                    if(error < 0)
                    {
                        testClient.setDutProperty(slot, "state", 3);
                        testClient.addDutError(slot, "Failed to load the sowtware");
                        logger.logError("Failed to load the sowtware into the chip flash memory for DUT " + testClient.dutNo(slot));
                        logger.logDebug("An error occured when downloading ZhagaSTD_software.hex for DUT " + testClient.dutNo(slot) + " Error code: " + error);
                    }

                    else
                    {
                        logger.logInfo("Software has been downloaded in DUT " + testClient.dutNo(slot));
                    }

                    jlink.close();
                }

                testClient.slotFullyTested(slot);
            }
        }
    },

    testDOUT: function ()
    {
        actionHintWidget.showProgressHint("Testing digital output...");

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];

                    testClient.setDOUT(slot, 1);
                    delay(100);
                    let response = testClient.railtestCommand(slot, "din");

                    testClient.clearDOUT(slot, 1);
                    delay(100);
                    let response2 = testClient.railtestCommand(slot, "din");
                    if(response.includes("state:1") && response2.includes("state:0"))
                    {
                        testClientList[i].setDutProperty(slot, "doutChecked", true);
                        logger.logSuccess("Digital output for DUT " + testClient.dutNo(slot) + " has been tested.");
                    }

                    else
                    {
                       logger.logError("Faled to test digital output for DUT " + testClient.dutNo(slot));
                    }
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    //---

    testRadio: function ()
    {
        GeneralCommands.testRadio(ZhagaSTD.RfModuleId, 19, 80, -50, 50, 7);
    },

    //---

    checkAinVoltage: function ()
    {
        for (var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (var i = 0; i < testClientList.length; i++)
            {
                let testClient = testClientList[i];
                let voltage = testClient.readAIN(slot, 1, 0);
                if(voltage > 70000 && voltage < 72000)
                {
                    testClientList[i].setDutProperty(slot, "voltageChecked", true);
                    logger.logSuccess("Voltage (3.3V) on AIN 1 for DUT " + testClientList[i].dutNo(slot) + " checked.");
                }
                else
                {
                    testClientList[i].setDutProperty(slot, "voltageChecked", false);
                    testClientList[i].addDutError(slot, response.join(' '));
                    logger.logDebug("Error voltage value on AIN 1 : " + voltage  + ".");
                    logger.logError("Error voltage value on AIN 1 is detected. DUT " + testClientList[i].dutNo(slot));
                }
            }
        }
    },

    //---

    startTesting: function ()
    {
        GeneralCommands.testConnection();
        ZhagaSTD.openTestClients();

        if(!GeneralCommands.isMethodCorrect)
            return;

        GeneralCommands.clearDutsInfo();
        GeneralCommands.detectDuts();
        GeneralCommands.detectDuts();
        GeneralCommands.detectDuts();
        ZhagaECO.downloadRailtest();
        GeneralCommands.readChipId();
        ZhagaECO.checkAinVoltage();
        GeneralCommands.testAccelerometer();
        GeneralCommands.testLightSensor();
        GeneralCommands.testDALI();
        GeneralCommands.testGNSS();
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
                    if( testClient.dutProperty(slot, "id") !== "" &&
                            testClient.dutProperty(slot, "voltageChecked") &&
                            testClient.dutProperty(slot, "lightSensChecked") &&
                            testClient.dutProperty(slot, "daliChecked") &&
                            testClient.dutProperty(slot, "gnssChecked") &&
                            testClient.dutProperty(slot, "radioChecked") &&
                            testClient.dutProperty(slot, "accelChecked")
                            )
                    {
                        testClient.setDutProperty(slot, "state", 2);
                    }

                    else
                    {
                        testClient.setDutProperty(slot, "state", 3);
                    }

//                    testClient.slotFullyTested(slot);
                }
            }
        }
    }
}

methodManager.addFunctionToGeneralList("Full cycle testing", ZhagaSTD.startTesting);
methodManager.addFunctionToGeneralList("Test connection to JLink", GeneralCommands.testConnection);
methodManager.addFunctionToGeneralList("Establish connection to sockets", ZhagaSTD.openTestClients);
methodManager.addFunctionToGeneralList("Clear previous test results for DUTs", GeneralCommands.clearDutsInfo);
methodManager.addFunctionToGeneralList("Detect DUTs", GeneralCommands.detectDuts);
methodManager.addFunctionToGeneralList("Download Railtest", ZhagaSTD.downloadRailtest);
methodManager.addFunctionToGeneralList("Read CSA", GeneralCommands.readCSA);
methodManager.addFunctionToGeneralList("Supply power to DUTs", GeneralCommands.powerOn);
methodManager.addFunctionToGeneralList("Power off DUTs", GeneralCommands.powerOff);
methodManager.addFunctionToGeneralList("Read unique device identifiers (ID)", GeneralCommands.readChipId);
methodManager.addFunctionToGeneralList("Check voltage on AIN 1 (3.3V)", ZhagaSTD.checkAinVoltage);
methodManager.addFunctionToGeneralList("Test digital output", ZhagaSTD.testDOUT);
methodManager.addFunctionToGeneralList("Test accelerometer", GeneralCommands.testAccelerometer);
methodManager.addFunctionToGeneralList("Test light sensor", GeneralCommands.testLightSensor);
methodManager.addFunctionToGeneralList("Test DALI", GeneralCommands.testDALI);
methodManager.addFunctionToGeneralList("Test radio interface", ZhagaSTD.testRadio);
methodManager.addFunctionToGeneralList("Test GNSS", GeneralCommands.testGNSS);
methodManager.addFunctionToGeneralList("Check Testing Completion", ZhagaSTD.checkTestingCompletion);
methodManager.addFunctionToGeneralList("Download Software", ZhagaSTD.downloadSoftware);
