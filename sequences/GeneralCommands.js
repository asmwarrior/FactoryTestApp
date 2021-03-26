const SLOTS_NUMBER = 3;
var jlinkList = [];
var testClientList = [];

function delay(milliseconds)
{
  const date = Date.now();
  let currentDate = null;
  do {
    currentDate = Date.now();
  } while (currentDate - date < milliseconds);
}

GeneralCommands =
{
    testConnection: function ()
    {
        for (var i = 0; i < jlinkList.length; i++)
        {
            jlinkList[i].establishConnection();
        }
    },

    //---

    openTestClients: function (portsIdList)
    {
        for (var i = 0; i < testClientList.length; i++)
        {
            let testClient = testClientList[i];
            testClient.open(portsIdList[i]);
        }
    },

    //---

    readCSA: function()
    {
        for (var i = 0; i < testClientList.length; i++)
        {
            let testClient = testClientList[i];
            logger.logInfo("Measuring board " + testClient.no() + " current: " + testClient.readCSA(0) + " mA");
        }
    },

    //---

    powerOn: function ()
    {
        for (var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (var i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    testClient.powerOn(slot);
                    logger.logInfo("DUT " + testClient.dutNo(slot) + " is switched ON");
                }
            }
        }
    },

    //---

    powerOff: function ()
    {
        for (var slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (var i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    testClient.powerOff(slot);
                    logger.logInfo("DUT " + testClient.dutNo(slot) + " is switched OFF");
                }
            }
        }
    },

    //---

    readTemperature: function ()
    {
        for (var i = 0; i < testClientList.length; i++)
        {
            let testClient = testClientList[i];
            logger.logInfo("Measuring board " + testClient.no() + " temperature: " + testClient.readTemperature());
        }
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

                testClient.setTimeout(500);
                testClient.powerOff(slot);
                testClient.resetDut(slot);
            }
        }
        delay(100);


        for (slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (i = 0; i < testClientList.length; i++)
            {
                var testClient = testClientList[i];

                if(!testClient.isConnected())
                    continue;

                testClient.setTimeout(500);
                logger.logDebug("Attempting connection to slot " + slot + " of board " + testClient.no() + "...");

                var prevCSA = testClient.readCSA(0);
                testClient.powerOn(slot);
                var currCSA = testClient.readCSA(0);

                if((currCSA - prevCSA) > 15)
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

    readChipId: function ()
    {
        actionHintWidget.showProgressHint("Reading device's IDs...");

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    let response = testClient.railtestCommand(slot, "getmemw 0x0FE081F0 2");
                    let id = response[response.length - 1].slice(2) + response[response.length - 3].slice(2);
                    testClient.setDutProperty(slot, "id", id.toUpperCase());
                    logger.logSuccess("ID for DUT " + testClient.dutNo(slot) + " has been read: " + testClient.dutProperty(slot, "id"));
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    testAccelerometer: function ()
    {
        actionHintWidget.showProgressHint("Testing Accelerometer...");

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    let response = testClient.railtestCommand(slot, "accl");
                    let patternFound = false;

                    if(response.length < 3)
                    {
                        testClient.setDutProperty(slot, "accelChecked", false);
                        testClient.addDutError(slot, response.join(' '));
                        logger.logError("Accelerometer failture for DUT " + testClient.dutNo(slot) + ". No response recieved!");
                    }

                    else
                    {
                        for (let j = 0; j < response.length; j++)
                        {
                            if((j + 2) < response.length)
                            {
                                if (response[j].includes("X") && response[j + 1].includes("Y") && response[j + 2].includes("Z"))
                                {
                                    patternFound = true;
                                    let x = Number(response[j].slice(2, 5));
                                    let y = Number(response[j + 1].slice(2, 5));
                                    let z = Number(response[j + 2].slice(2, 5));

                                    if (x > 10 || x < -10 || y > 10 || y < -10 || z < -90 || z > 100)
                                    {
                                        testClient.setDutProperty(slot, "accelChecked", false);
                                        testClient.addDutError(slot, response.join(' '));
                                        logger.logDebug("Accelerometer failure for DUT " + testClient.dutNo(slot) + "; X=" + x +", Y=" + y + ", Z=" + z + ".");
                                        logger.logError("Accelerometer failture for DUT " + testClient.dutNo(slot));
                                    }
                                    else
                                    {
                                        testClient.setDutProperty(slot, "accelChecked", true);
                                        logger.logSuccess("Accelerometer for DUT " + testClient.dutNo(slot) + " has been tested successfully.");
                                    }

                                    break;
                                }
                            }
                        }
                    }

                    if(!patternFound)
                    {
                        testClient.setDutProperty(slot, "accelChecked", false);
                        testClient.addDutError(slot, response.join(' '));
                        logger.logError("Accelerometer failture for DUT " + testClient.dutNo(slot) + ". Invalid response recieved!");
                        logger.logDebug("Accelerometer failure. Invalid response: " + response);
                    }
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    testLightSensor: function ()
    {
        actionHintWidget.showProgressHint("Testing light sensor...");

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    let response = testClient.railtestCommand(slot, "lsen");
                    if (response[2].includes("opwr"))
                    {
                           let x = Number(response[2].slice(5, 5));

                           if (x < 0)
                           {
                               testClientList.setDutProperty(slot, "lightSensChecked", false);
                               testClientList.addDutError(slot, response.join(' '));
                               logger.logDebug("Light sensor failure: X=" + x  + ".");
                               logger.logError("Light sensor failture for DUT " + testClient.dutNo(slot));
                           }
                           else
                           {
                               testClient.setDutProperty(slot, "lightSensChecked", true);
                               logger.logSuccess("Light sensor for DUT " + testClient.dutNo(slot) + " has been tested successfully.");
                           }
                    }

                    else
                    {
                        testClient.setDutProperty(slot, "lightSensChecked", false);
                        testClient.addDutError(slot, response.join(' '));
                        logger.logError("Light sensor failture for DUT " + testClient.dutNo(slot));
                    }
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    testRadio: function (RfModuleId)
    {
        actionHintWidget.showProgressHint("Testing radio interface...");

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    testClientList[i].testRadio(slot, RfModuleId);
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    },

    testDALI: function ()
    {
        actionHintWidget.showProgressHint("Testing DALI interface...");

        GeneralCommands.powerOff();

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
                        logger.logError("DALI testing for DUT " + testClient.dutNo(slot) + " has been failed!");
                        logger.logDebug("DALI failure: " + responseString  + ".");
                    }

                    testClient.railtestCommand(slot, "dali 0xFE80 16 0 0");
                    testClient.powerOff(slot);
                }
            }
        }

        for (i = 0; i < testClientList.length; i++)
        {
            testClientList[i].daliOff();
        }

        actionHintWidget.showProgressHint("READY");
    },

    testGNSS: function ()
    {
        actionHintWidget.showProgressHint("Testing GNSS module...");

        GeneralCommands.powerOn();
        delay(1000);

        for(let slot = 1; slot < SLOTS_NUMBER + 1; slot++)
        {
            for (let i = 0; i < testClientList.length; i++)
            {
                if(testClientList[i].isDutAvailable(slot) && testClientList[i].isDutChecked(slot))
                {
                    let testClient = testClientList[i];
                    let responseString = testClient.railtestCommand(slot, "gnrx 3").join(' ');
                    if (responseString.includes("line"))
                    {
                        testClient.setDutProperty(slot, "gnssChecked", true);
                        logger.logSuccess("GNSS module for DUT " + testClient.dutNo(slot) + " has been tested successfully.");
                    }

                    else
                    {
                        testClient.setDutProperty(slot, "gnssChecked", false);
                        testClient.addDutError(slot, responseString);
                        logger.logDebug("GNSS module failture: " + responseString + ".");
                        logger.logError("GNSS module failture for DUT " + testClient.dutNo(slot));

                    }
                }
            }
        }

        actionHintWidget.showProgressHint("READY");
    }
}
