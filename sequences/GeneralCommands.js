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
        jlink.establishConnection();
    },

    readCSA: function ()
    {
        testClient.readCSA(0);
        logger.logInfo("Measuring board " + testClient.no() + " current: " + testClient.currentCSA() + " mA");
    },

    //---

    startJlinkScript: function (scriptFile)
    {
        for (let slot = 1; slot < 4; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                testClient.powerOn(slot);
                testClient.delay(1000);
                testClient.switchSWD(slot);
                testClient.delay(100);
                jlink.startScript(scriptFile);
                testClient.delay(3000);
            }
        }
    },

    //---

    powerOn: function ()
    {
        for(var i = 1; i < testClient.dutsCount() + 1; i++)
        {
            if(testClient.isDutChecked(i))
            {
                testClient.powerOn(i);
                logger.logInfo("DUT " + testClient.dutNo(i) + " switched ON");
            }
        }
    },

    //---

    powerOff: function ()
    {
        for(var i = 1; i < testClient.dutsCount() + 1; i++)
        {
            if(testClient.isDutChecked(i))
            {
                testClient.powerOff(i);
                logger.logInfo("DUT " + testClient.dutNo(i) + " switched OFF");
            }
        }
    },

    //---

    detectDuts: function ()
    {
        testClient.commandSequenceStarted();

        testClient.setActive(false);

        for (var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            testClient.powerOff(slot);
        }

        testClient.delay(2000);

        for(slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            testClient.setCurrentSlot(slot);

            testClient.readCSA(0);
            testClient.delay(100);
            var currentCSA = testClient.currentCSA();

            testClient.powerOn(slot);
            testClient.delay(100);

            testClient.readCSA(0);
            testClient.delay(100);
            if((testClient.currentCSA() - currentCSA) > 15 && currentCSA !== -1)
            {
                logger.logSuccess("Device connected to the slot " + slot + " of the test board " + testClient.no());
                testClient.setDutProperty(slot, "state", 1);
                testClient.setDutProperty(slot, "checked", true);
                testClient.setActive(true);
            }

            else
            {
                testClient.setDutProperty(slot, "state", 0);
                testClient.setDutProperty(slot, "checked", false);
            }

            testClient.powerOff(slot);
            testClient.delay(2000);
        }

        testClient.commandSequenceFinished();
    },

    //---

    readChipId: function ()
    {
        for(var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                testClient.readChipId(slot);
            }
        }
    },

    testAccelerometer: function ()
    {
        for(var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                testClient.testAccelerometer(slot);
            }
        }
    },

    testLightSensor: function ()
    {
        for(var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                testClient.testLightSensor(slot);
            }
        }
    },

    testDALI: function ()
    {
        for(var i = 1; i < testClient.dutsCount() + 1; i++)
        {
            if(testClient.isDutChecked(i))
            {
                testClient.powerOff(i);
            }
        }

        testClient.daliOn();
        for(var slot = 1; slot < testClient.dutsCount() + 1; slot++)
        {
            if(testClient.isDutAvailable(slot) && testClient.isDutChecked(slot))
            {
                testClient.switchSWD(slot);
                testClient.powerOn(slot);
                delay(1000);

                testClient.testDALI();
                delay(500);

                testClient.powerOff(slot);
            }
        }
        testClient.daliOff();
    }
}
