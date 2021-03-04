GeneralCommands =
{
    readCSA: function ()
    {
        testClient.readCSA(0);
        logger.logInfo("Measuring board " + testClient.no() + " current: " + testClient.currentCSA() + " mA");
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
                testClient.setDutState(slot, 1);
                testClient.setDutChecked(slot, true);
                testClient.setActive(true);
            }

            else
            {
                testClient.setDutState(slot, 0);
                testClient.setDutChecked(slot, false);
            }

            testClient.powerOff(slot);
            testClient.delay(2000);
            testClient.currentDutChanged();
        }

        testClient.commandSequenceFinished();
    },

    //---

    readChipId: function ()
    {
        for(var i = 1; i < testClient.dutsCount() + 1; i++)
        {
            if(testClient.isDutAvailable(i) && testClient.isDutChecked(i))
            {
                testClient.readChipId(i);
                testClient.delay(2000);
            }
        }
    }
}
