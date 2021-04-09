var SLOTS_NUMBER = 3
methodManager.addMethod("OLC NemaPP");

NemaPP =
{
    measuringBoardIDs: ["5CDA66603935", "5CDA626D3431", "5CDD786A3431", "5CE0646D3431",  "5CDC766A3431"],
    RfModuleId: "A5XK3RJTA",

    //---

    openTestClients: function ()
    {
        GeneralCommands.openTestClients(NemaPP.measuringBoardIDs);
    },

    //---

    powerOn: function ()
    {
            for (var i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].no() === 5)
                {
                    testClientList[i].powerOn(1);
                    logger.logInfo("All connected DUTs are switched ON");
                    break;
                }
            }
    },

    //---

    powerOff: function ()
    {
            for (var i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].no() === 5)
                {
                    testClientList[i].powerOff(1);
                    logger.logInfo("All connected DUTs are switched OFF");
                    break;
                }
            }
    },

    //---

    downloadRailtest: function ()
    {
        GeneralCommands.downloadRailtest("sequences/OlcNemaPP/dummy_btl_efr32xg12.s37", "sequences/OlcNemaPP/railtest_nema.hex");
    },

    //---

    downloadSoftware: function ()
    {
        GeneralCommands.downloadSoftware("sequences/OlcNemaPP/nemaPP_software.hex");
    },

    //---

    detectDuts: function ()
    {
        actionHintWidget.showProgressHint("Detecting DUTs in the testing fixture...");
        NemaPP.powerOn();

        for (var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (i = 0; i < testClientList.length; i++)
            {
                var testClient = testClientList[i];

                if(!testClient.isConnected())
                    continue;

                testClient.setTimeout(300);

                let voltage = testClient.readAIN(slot, 4, 0);

                let counter = 0;
                while((voltage === 0 || voltage === -1) && (counter < 30))
                {
                    delay(200);
                    voltage = testClient.readAIN(slot, 4, 0);
                    console.log(testClientList[i].dutNo(slot) + " : " + voltage);
                    counter++;
                }

                if(voltage > 43000 && voltage < 52000)
                {
                    logger.logSuccess("Device connected to the slot " + slot + " of the test board " + testClient.no() + " detected.");
                    testClient.setDutProperty(slot, "state", 1);
                    testClient.setDutProperty(slot, "checked", true);
                }

                else
                {
                    testClient.setDutProperty(slot, "state", 0);
                    testClient.setDutProperty(slot, "checked", false);
                }

                testClient.setTimeout(10000);
            }
        }

        actionHintWidget.showProgressHint("READY");

    },

    //---

    checkAinVoltage: function ()
    {
        actionHintWidget.showProgressHint("Checking voltage on AIN1...");

        for (var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (var i = 0; i < testClientList.length; i++)
            {
                let testClient = testClientList[i];
                if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
                {
                    let voltage = testClient.readAIN(slot, 1, 0);
                    if(voltage > 70000 && voltage < 72000)
                    {
                        testClientList[i].setDutProperty(slot, "voltageChecked", true);
                        logger.logSuccess("Voltage (3.3V) on AIN 1 for DUT " + testClientList[i].dutNo(slot) + " is checked.");
                    }
                    else
                    {
                        testClientList[i].setDutProperty(slot, "voltageChecked", false);
                        testClientList[i].addDutError(slot, "Error voltage on AIN1");
                        logger.logDebug("Error voltage value on AIN 1 : " + voltage  + " for DUT " + testClientList[i].dutNo(slot));
                        logger.logError("Error voltage value on AIN 1 is detected for DUT " + testClientList[i].dutNo(slot));
                    }
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    //---

    testRTC: function ()
    {
        actionHintWidget.showProgressHint("Testing RTC...");

        let currentDate = new Date();
        let year = String(currentDate.getFullYear());
        year = year.slice(2);

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    testClient.railtestCommand(slot, "srtc" +
                                                     " " + year +
                                                     " " + currentDate.getMonth() +
                                                     " " + currentDate.getDate() +
                                                     " " + currentDate.getHours() +
                                                     " " + currentDate.getMinutes() +
                                                     " " + currentDate.getSeconds());
                }
            }
        }

        NemaPP.powerOff();
        delay(5000);
        NemaPP.powerOn();
        delay(1000);

        for(slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    let response = testClient.railtestCommand(slot, "rtc");
                    if (response.length < 7)
                        response = testClient.railtestCommand(slot, "rtc");

                    let responseString = response.join(' ');

                    if(responseString.includes("year:" + year))
                    {
                        testClientList[i].setDutProperty(slot, "rtcChecked", true);
                        logger.logSuccess("RTC module for DUT " + testClient.dutNo(slot) + " has been tested successfully.");
                    }

                    else
                    {
                        testClientList[i].setDutProperty(slot, "rtcChecked", false);
                        logger.logError("RTC module testing for DUT " + testClient.dutNo(slot) + " has been failed.");
                        logger.logDebug("RTC value for DUT " + testClient.dutNo(slot) + ": " + response.slice(7));
                    }
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    //---

    powerTable: [   -100, -100, -100,
                    -50, -100, -100,
                    -100, -100, -100,
                    -100, -100, -100,
                    -100, -100, -100  ],

    testRadio: function ()
    {
        GeneralCommands.testRadio(NemaPP.RfModuleId, 19, NemaPP.powerTable, -90, 0, 50);
    },

    testRadioDebug: function ()
    {
        logger.logDebug("Starting radio debugging cycle...");
        for (let power = 80; power >= -200; power -= 20)
        {
            let table = [power, power, power,
                         power, power, power,
                         power, power, power,
                         power, power, power,
                         power, power, power];

            GeneralCommands.testRadio(NemaPP.RfModuleId, 19, table, -60, 60, 200);
        }

    },

    //---

    testDALI: function ()
    {
        actionHintWidget.showProgressHint("Testing DALI interface...");

        for (var i = 0; i < testClientList.length; i++)
        {
            testClientList[i].daliOn();
        }

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    testClient.switchSWD(slot);
                    testClient.powerOn(slot);
                    delay(1000);

                    testClient.railtestCommand(slot, "dali 0xFE80 16 0 0");
                    let responseString = testClient.railtestCommand(slot, "dali 0xFF90 16 0 1000000").join(' ');

                    if(responseString.includes("error:0"))
                    {
                        testClient.setDutProperty(slot, "daliChecked", true);
                        logger.logSuccess("DALI interface for DUT " + testClient.dutNo(slot) + " has been tested successfully.");
                    }

                    else
                    {
                        testClient.setDutProperty(slot, "daliChecked", false);
                        testClient.addDutError(slot, responseString);
                        logger.logError("DALI testing for DUT " + testClient.dutNo(slot) + " has been failed.");
                        logger.logDebug("DALI failure: " + responseString  + ".");
                    }

                    testClient.railtestCommand(slot, "dali 0xFE80 16 0 0");
                }
            }
        }

        for (i = 0; i < testClientList.length; i++)
        {
            testClientList[i].daliOff();
        }

        actionHintWidget.showProgressHint("READY");
    },

    //---

    test12V: function ()
    {
        actionHintWidget.showProgressHint("Testing 12V output...");

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];

//                    testClient.railtestCommand(slot, "12vout 0");
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    //---

    startTesting: function ()
    {
        GeneralCommands.testConnection();
        NemaPP.openTestClients();

        if(!GeneralCommands.isMethodCorrect)
            return;

        GeneralCommands.clearDutsInfo();
        NemaPP.detectDuts();
        NemaPP.downloadRailtest();
        GeneralCommands.readChipId();
        GeneralCommands.testDALI();
        NemaPP.testRTC();
        NemaPP.checkAinVoltage();
        GeneralCommands.testAccelerometer();
        NemaPP.testRadio();
        NemaPP.checkTestingCompletion();
        NemaPP.downloadSoftware();
        NemaPP.powerOff();
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
                            testClient.dutProperty(slot, "daliChecked") &&
                            testClient.dutProperty(slot, "radioChecked") &&
                            testClient.dutProperty(slot, "rtcChecked") &&
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

                else if(testClient.isDutAvailable(slot) && !testClient.dutProperty(slot, "railtestDownloaded"))
                {
                    testClient.setDutProperty(slot, "checked", true);
                    testClient.setDutProperty(slot, "state", 3);
                }
            }
        }
    }
}

methodManager.addFunctionToGeneralList("Full cycle testing", NemaPP.startTesting);
methodManager.addFunctionToGeneralList("Test connection to JLink", GeneralCommands.testConnection);
methodManager.addFunctionToGeneralList("Establish connection to sockets", NemaPP.openTestClients);
methodManager.addFunctionToGeneralList("Clear previous test results for DUTs", GeneralCommands.clearDutsInfo);
methodManager.addFunctionToGeneralList("Detect DUTs", NemaPP.detectDuts);
methodManager.addFunctionToGeneralList("Download Railtest", NemaPP.downloadRailtest);
methodManager.addFunctionToGeneralList("Read CSA", GeneralCommands.readCSA);
methodManager.addFunctionToGeneralList("Read Temperature", GeneralCommands.readTemperature);
methodManager.addFunctionToGeneralList("Supply power to DUTs", NemaPP.powerOn);
//methodManager.addFunctionToGeneralList("Test radio debug", NemaPP.testRadioDebug);
methodManager.addFunctionToGeneralList("Power off DUTs", NemaPP.powerOff);
methodManager.addFunctionToGeneralList("Read unique device identifiers (ID)", GeneralCommands.readChipId);
methodManager.addFunctionToGeneralList("Read Real time clock (RTC) values", GeneralCommands.readRTC);
methodManager.addFunctionToGeneralList("Test Real time clock (RTC) module", NemaPP.testRTC);
methodManager.addFunctionToGeneralList("Check voltage on AIN 1 (3.3V)", NemaPP.checkAinVoltage);
methodManager.addFunctionToGeneralList("Test accelerometer", GeneralCommands.testAccelerometer);
methodManager.addFunctionToGeneralList("Test radio interface", NemaPP.testRadio);
methodManager.addFunctionToGeneralList("Test DALI", NemaPP.testDALI);
methodManager.addFunctionToGeneralList("Test 12V output", NemaPP.test12V);
methodManager.addFunctionToGeneralList("Check Testing Completion", NemaPP.checkTestingCompletion);
methodManager.addFunctionToGeneralList("Download Software", NemaPP.downloadSoftware);
