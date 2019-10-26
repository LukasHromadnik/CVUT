<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="XLXN_24" />
        <signal name="XLXN_26" />
        <signal name="XLXN_34" />
        <signal name="XLXN_36" />
        <signal name="XLXN_3" />
        <signal name="XLXN_39" />
        <signal name="MOORE1" />
        <signal name="XLXN_41" />
        <signal name="MOORE2" />
        <signal name="MEAL2">
        </signal>
        <signal name="MEAL1">
        </signal>
        <signal name="MEAL0">
        </signal>
        <signal name="EN">
        </signal>
        <signal name="B8" />
        <signal name="XLXN_49" />
        <signal name="CLEAR" />
        <signal name="XLXN_51" />
        <signal name="MOORE0" />
        <signal name="XLXN_58" />
        <signal name="XLXN_59" />
        <signal name="XLXN_60" />
        <signal name="D0" />
        <signal name="D1" />
        <signal name="D2" />
        <signal name="D3" />
        <signal name="D4" />
        <signal name="D5" />
        <signal name="D6" />
        <signal name="D7" />
        <port polarity="Input" name="B8" />
        <port polarity="Input" name="CLEAR" />
        <port polarity="Output" name="D0" />
        <port polarity="Output" name="D1" />
        <port polarity="Output" name="D2" />
        <port polarity="Output" name="D3" />
        <port polarity="Output" name="D4" />
        <port polarity="Output" name="D5" />
        <port polarity="Output" name="D6" />
        <port polarity="Output" name="D7" />
        <blockdef name="Q0">
            <timestamp>2014-3-10T12:55:12</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="Q1">
            <timestamp>2014-3-10T12:55:37</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="Q2">
            <timestamp>2014-3-10T12:55:19</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="fdc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-128" y2="-128" x1="0" />
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="64" y1="-256" y2="-256" x1="0" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <rect width="256" x="64" y="-320" height="256" />
            <line x2="80" y1="-112" y2="-128" x1="64" />
            <line x2="64" y1="-128" y2="-144" x1="80" />
            <line x2="192" y1="-64" y2="-32" x1="192" />
            <line x2="64" y1="-32" y2="-32" x1="192" />
        </blockdef>
        <blockdef name="d3_8e">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-576" y2="-576" x1="0" />
            <line x2="64" y1="-512" y2="-512" x1="0" />
            <line x2="64" y1="-448" y2="-448" x1="0" />
            <line x2="320" y1="-576" y2="-576" x1="384" />
            <line x2="320" y1="-512" y2="-512" x1="384" />
            <line x2="320" y1="-448" y2="-448" x1="384" />
            <line x2="320" y1="-384" y2="-384" x1="384" />
            <line x2="320" y1="-320" y2="-320" x1="384" />
            <line x2="320" y1="-256" y2="-256" x1="384" />
            <line x2="320" y1="-192" y2="-192" x1="384" />
            <line x2="320" y1="-128" y2="-128" x1="384" />
            <rect width="256" x="64" y="-640" height="576" />
            <line x2="64" y1="-128" y2="-128" x1="0" />
        </blockdef>
        <blockdef name="vcc">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-64" x1="64" />
            <line x2="64" y1="0" y2="-32" x1="64" />
            <line x2="32" y1="-64" y2="-64" x1="96" />
        </blockdef>
        <block symbolname="Q0" name="XLXI_1">
            <blockpin signalname="MEAL0" name="q0" />
            <blockpin signalname="MEAL1" name="q1" />
            <blockpin signalname="MEAL2" name="q2" />
            <blockpin signalname="EN" name="en" />
            <blockpin signalname="MOORE0" name="f" />
        </block>
        <block symbolname="Q1" name="XLXI_2">
            <blockpin signalname="MEAL0" name="q0" />
            <blockpin signalname="MEAL1" name="q1" />
            <blockpin signalname="MEAL2" name="q2" />
            <blockpin signalname="EN" name="en" />
            <blockpin signalname="MOORE1" name="f" />
        </block>
        <block symbolname="Q2" name="XLXI_3">
            <blockpin signalname="MEAL0" name="q0" />
            <blockpin signalname="MEAL1" name="q1" />
            <blockpin signalname="MEAL2" name="q2" />
            <blockpin signalname="EN" name="en" />
            <blockpin signalname="MOORE2" name="f" />
        </block>
        <block symbolname="fdc" name="XLXI_8">
            <blockpin signalname="B8" name="C" />
            <blockpin signalname="CLEAR" name="CLR" />
            <blockpin signalname="MOORE0" name="D" />
            <blockpin signalname="MEAL0" name="Q" />
        </block>
        <block symbolname="fdc" name="XLXI_9">
            <blockpin signalname="B8" name="C" />
            <blockpin signalname="CLEAR" name="CLR" />
            <blockpin signalname="MOORE1" name="D" />
            <blockpin signalname="MEAL1" name="Q" />
        </block>
        <block symbolname="fdc" name="XLXI_10">
            <blockpin signalname="B8" name="C" />
            <blockpin signalname="CLEAR" name="CLR" />
            <blockpin signalname="MOORE2" name="D" />
            <blockpin signalname="MEAL2" name="Q" />
        </block>
        <block symbolname="d3_8e" name="XLXI_25">
            <blockpin signalname="MEAL0" name="A0" />
            <blockpin signalname="MEAL1" name="A1" />
            <blockpin signalname="MEAL2" name="A2" />
            <blockpin signalname="XLXN_60" name="E" />
            <blockpin signalname="D0" name="D0" />
            <blockpin signalname="D1" name="D1" />
            <blockpin signalname="D2" name="D2" />
            <blockpin signalname="D3" name="D3" />
            <blockpin signalname="D4" name="D4" />
            <blockpin signalname="D5" name="D5" />
            <blockpin signalname="D6" name="D6" />
            <blockpin signalname="D7" name="D7" />
        </block>
        <block symbolname="vcc" name="XLXI_28">
            <blockpin signalname="XLXN_60" name="P" />
        </block>
        <block symbolname="vcc" name="XLXI_29">
            <blockpin signalname="EN" name="P" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="1296" y="368" name="XLXI_1" orien="R0">
        </instance>
        <instance x="1296" y="768" name="XLXI_2" orien="R0">
        </instance>
        <instance x="1296" y="1168" name="XLXI_3" orien="R0">
        </instance>
        <instance x="1296" y="1728" name="XLXI_8" orien="R0" />
        <instance x="1280" y="2096" name="XLXI_9" orien="R0" />
        <instance x="1264" y="2464" name="XLXI_10" orien="R0" />
        <branch name="MEAL2">
            <wire x2="1296" y1="272" y2="272" x1="1232" />
            <wire x2="1232" y1="272" y2="672" x1="1232" />
            <wire x2="1296" y1="672" y2="672" x1="1232" />
            <wire x2="1232" y1="672" y2="1072" x1="1232" />
            <wire x2="1296" y1="1072" y2="1072" x1="1232" />
            <wire x2="1232" y1="1072" y2="1232" x1="1232" />
            <wire x2="1712" y1="1232" y2="1232" x1="1232" />
            <wire x2="1712" y1="1232" y2="2208" x1="1712" />
            <wire x2="2048" y1="2208" y2="2208" x1="1712" />
            <wire x2="1712" y1="2208" y2="2208" x1="1648" />
            <wire x2="2544" y1="656" y2="656" x1="2048" />
            <wire x2="2048" y1="656" y2="2208" x1="2048" />
        </branch>
        <branch name="MEAL1">
            <wire x2="1296" y1="208" y2="208" x1="1248" />
            <wire x2="1248" y1="208" y2="608" x1="1248" />
            <wire x2="1296" y1="608" y2="608" x1="1248" />
            <wire x2="1248" y1="608" y2="832" x1="1248" />
            <wire x2="1248" y1="832" y2="1008" x1="1248" />
            <wire x2="1296" y1="1008" y2="1008" x1="1248" />
            <wire x2="1840" y1="832" y2="832" x1="1248" />
            <wire x2="1840" y1="832" y2="1840" x1="1840" />
            <wire x2="2064" y1="1840" y2="1840" x1="1840" />
            <wire x2="2304" y1="1840" y2="1840" x1="2064" />
            <wire x2="1840" y1="1840" y2="1840" x1="1664" />
            <wire x2="2304" y1="592" y2="1840" x1="2304" />
            <wire x2="2544" y1="592" y2="592" x1="2304" />
        </branch>
        <branch name="MEAL0">
            <wire x2="1296" y1="144" y2="144" x1="1280" />
            <wire x2="1280" y1="144" y2="544" x1="1280" />
            <wire x2="1296" y1="544" y2="544" x1="1280" />
            <wire x2="1280" y1="544" y2="944" x1="1280" />
            <wire x2="1296" y1="944" y2="944" x1="1280" />
            <wire x2="1280" y1="944" y2="1216" x1="1280" />
            <wire x2="1696" y1="1216" y2="1216" x1="1280" />
            <wire x2="1696" y1="1216" y2="1472" x1="1696" />
            <wire x2="2064" y1="1472" y2="1472" x1="1696" />
            <wire x2="1696" y1="1472" y2="1472" x1="1680" />
            <wire x2="2544" y1="528" y2="528" x1="2064" />
            <wire x2="2064" y1="528" y2="1472" x1="2064" />
        </branch>
        <branch name="EN">
            <wire x2="752" y1="336" y2="336" x1="528" />
            <wire x2="1296" y1="336" y2="336" x1="752" />
            <wire x2="752" y1="336" y2="736" x1="752" />
            <wire x2="1296" y1="736" y2="736" x1="752" />
            <wire x2="752" y1="736" y2="1136" x1="752" />
            <wire x2="1296" y1="1136" y2="1136" x1="752" />
        </branch>
        <branch name="B8">
            <wire x2="304" y1="1600" y2="1600" x1="272" />
            <wire x2="688" y1="1600" y2="1600" x1="304" />
            <wire x2="768" y1="1600" y2="1600" x1="688" />
            <wire x2="1296" y1="1600" y2="1600" x1="768" />
            <wire x2="768" y1="1600" y2="1968" x1="768" />
            <wire x2="1280" y1="1968" y2="1968" x1="768" />
            <wire x2="768" y1="1968" y2="2336" x1="768" />
            <wire x2="1264" y1="2336" y2="2336" x1="768" />
        </branch>
        <branch name="CLEAR">
            <wire x2="368" y1="2272" y2="2432" x1="368" />
            <wire x2="1248" y1="2432" y2="2432" x1="368" />
            <wire x2="1264" y1="2432" y2="2432" x1="1248" />
            <wire x2="1264" y1="2064" y2="2064" x1="1248" />
            <wire x2="1280" y1="2064" y2="2064" x1="1264" />
            <wire x2="1248" y1="2064" y2="2432" x1="1248" />
            <wire x2="1264" y1="1696" y2="2064" x1="1264" />
            <wire x2="1296" y1="1696" y2="1696" x1="1264" />
        </branch>
        <iomarker fontsize="28" x="272" y="1600" name="B8" orien="R180" />
        <iomarker fontsize="28" x="368" y="2272" name="CLEAR" orien="R270" />
        <instance x="2544" y="1104" name="XLXI_25" orien="R0" />
        <branch name="MOORE2">
            <wire x2="992" y1="1568" y2="2208" x1="992" />
            <wire x2="1184" y1="2208" y2="2208" x1="992" />
            <wire x2="1264" y1="2208" y2="2208" x1="1184" />
            <wire x2="1184" y1="2080" y2="2208" x1="1184" />
            <wire x2="1728" y1="2080" y2="2080" x1="1184" />
            <wire x2="1728" y1="944" y2="944" x1="1680" />
            <wire x2="1728" y1="944" y2="2080" x1="1728" />
            <wire x2="2000" y1="944" y2="944" x1="1728" />
        </branch>
        <branch name="MOORE1">
            <wire x2="1216" y1="1520" y2="1520" x1="848" />
            <wire x2="1216" y1="1520" y2="1840" x1="1216" />
            <wire x2="1280" y1="1840" y2="1840" x1="1216" />
            <wire x2="1216" y1="1840" y2="2128" x1="1216" />
            <wire x2="1744" y1="2128" y2="2128" x1="1216" />
            <wire x2="1744" y1="544" y2="544" x1="1680" />
            <wire x2="1744" y1="544" y2="2128" x1="1744" />
            <wire x2="2016" y1="544" y2="544" x1="1744" />
        </branch>
        <branch name="MOORE0">
            <wire x2="1232" y1="1472" y2="1472" x1="864" />
            <wire x2="1296" y1="1472" y2="1472" x1="1232" />
            <wire x2="1232" y1="1472" y2="1760" x1="1232" />
            <wire x2="1760" y1="1760" y2="1760" x1="1232" />
            <wire x2="1760" y1="144" y2="144" x1="1680" />
            <wire x2="1760" y1="144" y2="1760" x1="1760" />
            <wire x2="2032" y1="144" y2="144" x1="1760" />
        </branch>
        <branch name="XLXN_60">
            <wire x2="2544" y1="976" y2="976" x1="2480" />
            <wire x2="2480" y1="976" y2="1104" x1="2480" />
            <wire x2="2544" y1="1104" y2="1104" x1="2480" />
            <wire x2="2544" y1="1104" y2="1488" x1="2544" />
            <wire x2="2544" y1="1488" y2="1504" x1="2544" />
            <wire x2="2640" y1="1504" y2="1504" x1="2544" />
            <wire x2="2640" y1="1488" y2="1504" x1="2640" />
        </branch>
        <instance x="2576" y="1488" name="XLXI_28" orien="R0" />
        <branch name="D0">
            <wire x2="2960" y1="528" y2="528" x1="2928" />
        </branch>
        <iomarker fontsize="28" x="2960" y="528" name="D0" orien="R0" />
        <branch name="D1">
            <wire x2="2960" y1="592" y2="592" x1="2928" />
        </branch>
        <iomarker fontsize="28" x="2960" y="592" name="D1" orien="R0" />
        <branch name="D2">
            <wire x2="2960" y1="656" y2="656" x1="2928" />
        </branch>
        <iomarker fontsize="28" x="2960" y="656" name="D2" orien="R0" />
        <branch name="D3">
            <wire x2="2960" y1="720" y2="720" x1="2928" />
        </branch>
        <iomarker fontsize="28" x="2960" y="720" name="D3" orien="R0" />
        <branch name="D4">
            <wire x2="2960" y1="784" y2="784" x1="2928" />
        </branch>
        <iomarker fontsize="28" x="2960" y="784" name="D4" orien="R0" />
        <branch name="D5">
            <wire x2="2960" y1="848" y2="848" x1="2928" />
        </branch>
        <iomarker fontsize="28" x="2960" y="848" name="D5" orien="R0" />
        <branch name="D6">
            <wire x2="2960" y1="912" y2="912" x1="2928" />
        </branch>
        <iomarker fontsize="28" x="2960" y="912" name="D6" orien="R0" />
        <branch name="D7">
            <wire x2="2960" y1="976" y2="976" x1="2928" />
        </branch>
        <iomarker fontsize="28" x="2960" y="976" name="D7" orien="R0" />
        <instance x="464" y="336" name="XLXI_29" orien="R0" />
    </sheet>
</drawing>