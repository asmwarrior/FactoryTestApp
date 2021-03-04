GeneralCommands =
{
    readCSA: function ()
    {
        testClientList.forEach(
        function(item)
        {
            item.readCSA(0);
            mainWindow.delay(50);
            logger.logInfo("Measuring board " + item.no() + " current: " + item.currentCSA() + " mA");
        });
    },

    //---

    powerOn: function ()
    {
        testClientList.forEach(
        function(item)
        {
            for(var i = 1; i < item.dutsCount() + 1; i++)
            {
                if(item.isDutChecked(i))
                {
                    item.powerOn(i);
                    logger.logInfo("DUT " + item.dutNo(i) + " switched ON");
                }
            }
        });
    },

    //---

    powerOff: function ()
    {
        testClientList.forEach(
        function(item)
        {
            for(var i = 1; i < item.dutsCount() + 1; i++)
            {
                if(item.isDutChecked(i))
                {
                    item.powerOff(i);
                    logger.logInfo("DUT " + item.dutNo(i) + " switched OFF");
                }
            }
        });
    },

    //---

    detectDuts: function ()
    {
        testClientList.forEach(
        function(item)
        {
            item.checkDutsCurrent();
        });
    },

    //---

    readChipId: function ()
    {
        testClientList.forEach(
        function(item)
        {
            for(var i = 1; i < item.dutsCount() + 1; i++)
            {
                if(item.isDutAvailable(i) && item.isDutChecked(i))
                {
                    item.readChipId(i);
                    mainWindow.delay(2000);
                }
            }
        });
    }
}
