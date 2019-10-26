<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="XLXN_1" />
        <signal name="XLXN_2" />
        <signal name="XLXN_12" />
        <signal name="F16" />
        <signal name="F4" />
        <signal name="XLXN_6" />
        <signal name="XLXN_7" />
        <signal name="F8" />
        <signal name="XLXN_9" />
        <signal name="F2" />
        <signal name="CLK" />
        <port polarity="Output" name="F16" />
        <port polarity="Output" name="F4" />
        <port polarity="Output" name="F8" />
        <port polarity="Output" name="F2" />
        <port polarity="Input" name="CLK" />
        <blockdef name="klopD">
            <timestamp>2014-3-31T7:3:18</timestamp>
            <rect width="256" x="64" y="-128" height="128" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
        </blockdef>
        <block symbolname="klopD" name="XLXI_7">
            <blockpin signalname="XLXN_2" name="D" />
            <blockpin signalname="XLXN_9" name="E" />
            <blockpin signalname="F4" name="Q1" />
            <blockpin signalname="XLXN_2" name="Q2" />
        </block>
        <block symbolname="klopD" name="XLXI_9">
            <blockpin signalname="XLXN_12" name="D" />
            <blockpin signalname="XLXN_7" name="E" />
            <blockpin signalname="F16" name="Q1" />
            <blockpin signalname="XLXN_12" name="Q2" />
        </block>
        <block symbolname="klopD" name="XLXI_8">
            <blockpin signalname="XLXN_7" name="D" />
            <blockpin signalname="XLXN_2" name="E" />
            <blockpin signalname="F8" name="Q1" />
            <blockpin signalname="XLXN_7" name="Q2" />
        </block>
        <block symbolname="klopD" name="XLXI_6">
            <blockpin signalname="XLXN_9" name="D" />
            <blockpin signalname="CLK" name="E" />
            <blockpin signalname="F2" name="Q1" />
            <blockpin signalname="XLXN_9" name="Q2" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="1456" y="1328" name="XLXI_7" orien="R0">
        </instance>
        <instance x="2384" y="1328" name="XLXI_9" orien="R0">
        </instance>
        <branch name="XLXN_2">
            <wire x2="1376" y1="1136" y2="1232" x1="1376" />
            <wire x2="1456" y1="1232" y2="1232" x1="1376" />
            <wire x2="1904" y1="1136" y2="1136" x1="1376" />
            <wire x2="1904" y1="1136" y2="1296" x1="1904" />
            <wire x2="1904" y1="1296" y2="1536" x1="1904" />
            <wire x2="1728" y1="1536" y2="1680" x1="1728" />
            <wire x2="1792" y1="1680" y2="1680" x1="1728" />
            <wire x2="1904" y1="1536" y2="1536" x1="1728" />
            <wire x2="1904" y1="1296" y2="1296" x1="1840" />
        </branch>
        <branch name="XLXN_12">
            <wire x2="2320" y1="1136" y2="1232" x1="2320" />
            <wire x2="2384" y1="1232" y2="1232" x1="2320" />
            <wire x2="2832" y1="1136" y2="1136" x1="2320" />
            <wire x2="2832" y1="1136" y2="1296" x1="2832" />
            <wire x2="2832" y1="1296" y2="1296" x1="2768" />
        </branch>
        <branch name="F16">
            <wire x2="2848" y1="864" y2="864" x1="2768" />
            <wire x2="2848" y1="864" y2="1232" x1="2848" />
            <wire x2="2848" y1="1232" y2="1232" x1="2768" />
        </branch>
        <branch name="F4">
            <wire x2="1856" y1="864" y2="864" x1="1840" />
            <wire x2="1856" y1="864" y2="1232" x1="1856" />
            <wire x2="1856" y1="1232" y2="1232" x1="1840" />
        </branch>
        <instance x="1792" y="1712" name="XLXI_8" orien="R0">
        </instance>
        <branch name="XLXN_7">
            <wire x2="1712" y1="1520" y2="1616" x1="1712" />
            <wire x2="1792" y1="1616" y2="1616" x1="1712" />
            <wire x2="2368" y1="1520" y2="1520" x1="1712" />
            <wire x2="2272" y1="1680" y2="1680" x1="2176" />
            <wire x2="2272" y1="1296" y2="1680" x1="2272" />
            <wire x2="2368" y1="1296" y2="1296" x1="2272" />
            <wire x2="2384" y1="1296" y2="1296" x1="2368" />
            <wire x2="2368" y1="1296" y2="1520" x1="2368" />
        </branch>
        <branch name="F8">
            <wire x2="2192" y1="864" y2="864" x1="2176" />
            <wire x2="2192" y1="864" y2="1616" x1="2192" />
            <wire x2="2192" y1="1616" y2="1616" x1="2176" />
        </branch>
        <instance x="960" y="1328" name="XLXI_6" orien="R0">
        </instance>
        <branch name="XLXN_9">
            <wire x2="896" y1="1152" y2="1232" x1="896" />
            <wire x2="960" y1="1232" y2="1232" x1="896" />
            <wire x2="1408" y1="1152" y2="1152" x1="896" />
            <wire x2="1408" y1="1152" y2="1296" x1="1408" />
            <wire x2="1456" y1="1296" y2="1296" x1="1408" />
            <wire x2="1408" y1="1296" y2="1296" x1="1344" />
        </branch>
        <branch name="F2">
            <wire x2="1360" y1="848" y2="848" x1="1344" />
            <wire x2="1360" y1="848" y2="1232" x1="1360" />
            <wire x2="1360" y1="1232" y2="1232" x1="1344" />
        </branch>
        <branch name="CLK">
            <wire x2="960" y1="1296" y2="1296" x1="928" />
        </branch>
        <iomarker fontsize="28" x="928" y="1296" name="CLK" orien="R180" />
        <iomarker fontsize="28" x="1344" y="848" name="F2" orien="R180" />
        <iomarker fontsize="28" x="1840" y="864" name="F4" orien="R180" />
        <iomarker fontsize="28" x="2176" y="864" name="F8" orien="R180" />
        <iomarker fontsize="28" x="2768" y="864" name="F16" orien="R180" />
    </sheet>
</drawing>