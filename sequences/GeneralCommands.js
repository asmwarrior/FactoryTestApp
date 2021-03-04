GeneralCommands =
{
    readCSA: function ()
    {
        testClientList.forEach(
        function(item)
        {
            item.readCSA(0);
//            item.waitCommandFinished();
            logger.logInfo("Measuring board " + item.no() + " current: " + item.currentCSA() + " mA");
        });
    }
}
