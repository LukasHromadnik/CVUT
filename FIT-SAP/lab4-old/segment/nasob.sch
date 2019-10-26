<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="XLXN_2" />
        <signal name="XLXN_3" />
        <signal name="XLXN_4" />
        <signal name="XLXN_5" />
        <signal name="XLXN_7" />
        <signal name="XLXN_9" />
        <signal name="XLXN_11" />
        <signal name="XLXN_12" />
        <signal name="XLXN_13" />
        <signal name="XLXN_14" />
        <signal name="XLXN_15" />
        <signal name="XLXN_16" />
        <signal name="XLXN_17" />
        <signal name="XLXN_18" />
        <signal name="EN" />
        <signal name="XLXN_20" />
        <signal name="XLXN_21" />
        <signal name="CLEAR" />
        <signal name="XLXN_23" />
        <signal name="CLOCK" />
        <signal name="TL1" />
        <port polarity="Input" name="EN" />
        <port polarity="Input" name="CLEAR" />
        <port polarity="Input" name="CLOCK" />
        <port polarity="Input" name="TL1" />
        <blockdef name="q0">
            <timestamp>2014-3-10T12:52:30</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="q1">
            <timestamp>2014-3-10T12:52:36</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="q2">
            <timestamp>2014-3-10T12:52:43</timestamp>
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
        <blockdef name="debounce">
            <timestamp>2014-3-10T12:56:21</timestamp>
            <rect width="256" x="64" y="-128" height="128" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
        </blockdef>
        <block symbolname="q0" name="XLXI_1">
            <blockpin signalname="EN" name="d" />
            <blockpin signalname="XLXN_18" name="c" />
            <blockpin signalname="XLXN_16" name="b" />
            <blockpin signalname="XLXN_11" name="a" />
            <blockpin signalname="XLXN_5" name="f" />
        </block>
        <block symbolname="fdc" name="XLXI_6">
            <blockpin signalname="XLXN_2" name="C" />
            <blockpin signalname="CLEAR" name="CLR" />
            <blockpin signalname="XLXN_5" name="D" />
            <blockpin signalname="XLXN_11" name="Q" />
        </block>
        <block symbolname="q1" name="XLXI_7">
            <blockpin signalname="EN" name="d" />
            <blockpin signalname="XLXN_18" name="c" />
            <blockpin signalname="XLXN_16" name="b" />
            <blockpin signalname="XLXN_11" name="a" />
            <blockpin signalname="XLXN_7" name="f" />
        </block>
        <block symbolname="fdc" name="XLXI_8">
            <blockpin signalname="XLXN_2" name="C" />
            <blockpin signalname="CLEAR" name="CLR" />
            <blockpin signalname="XLXN_7" name="D" />
            <blockpin signalname="XLXN_16" name="Q" />
        </block>
        <block symbolname="q2" name="XLXI_9">
            <blockpin signalname="EN" name="d" />
            <blockpin signalname="XLXN_18" name="c" />
            <blockpin signalname="XLXN_16" name="b" />
            <blockpin signalname="XLXN_11" name="a" />
            <blockpin signalname="XLXN_9" name="f" />
        </block>
        <block symbolname="fdc" name="XLXI_10">
            <blockpin signalname="XLXN_2" name="C" />
            <blockpin signalname="CLEAR" name="CLR" />
            <blockpin signalname="XLXN_9" name="D" />
            <blockpin signalname="XLXN_18" name="Q" />
        </block>
        <block symbolname="debounce" name="XLXI_11">
            <blockpin signalname="CLOCK" name="clk" />
            <blockpin signalname="TL1" name="tl_in" />
            <blockpin signalname="XLXN_2" name="tl_out" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="816" y="448" name="XLXI_1" orien="R0">
        </instance>
        <instance x="816" y="848" name="XLXI_6" orien="R0" />
        <instance x="816" y="1184" name="XLXI_7" orien="R0">
        </instance>
        <instance x="816" y="1584" name="XLXI_8" orien="R0" />
        <instance x="816" y="1888" name="XLXI_9" orien="R0">
        </instance>
        <instance x="816" y="2288" name="XLXI_10" orien="R0" />
        <instance x="96" y="944" name="XLXI_11" orien="R0">
        </instance>
        <branch name="XLXN_2">
            <wire x2="640" y1="848" y2="848" x1="480" />
            <wire x2="640" y1="848" y2="1456" x1="640" />
            <wire x2="816" y1="1456" y2="1456" x1="640" />
            <wire x2="640" y1="1456" y2="2160" x1="640" />
            <wire x2="816" y1="2160" y2="2160" x1="640" />
            <wire x2="640" y1="720" y2="848" x1="640" />
            <wire x2="816" y1="720" y2="720" x1="640" />
        </branch>
        <branch name="XLXN_5">
            <wire x2="752" y1="464" y2="592" x1="752" />
            <wire x2="816" y1="592" y2="592" x1="752" />
            <wire x2="1280" y1="464" y2="464" x1="752" />
            <wire x2="1280" y1="224" y2="224" x1="1200" />
            <wire x2="1280" y1="224" y2="464" x1="1280" />
        </branch>
        <branch name="XLXN_7">
            <wire x2="816" y1="1328" y2="1328" x1="736" />
            <wire x2="736" y1="1328" y2="1616" x1="736" />
            <wire x2="1280" y1="1616" y2="1616" x1="736" />
            <wire x2="1280" y1="960" y2="960" x1="1200" />
            <wire x2="1280" y1="960" y2="1616" x1="1280" />
        </branch>
        <branch name="XLXN_9">
            <wire x2="752" y1="1904" y2="2032" x1="752" />
            <wire x2="816" y1="2032" y2="2032" x1="752" />
            <wire x2="1280" y1="1904" y2="1904" x1="752" />
            <wire x2="1280" y1="1664" y2="1664" x1="1200" />
            <wire x2="1280" y1="1664" y2="1904" x1="1280" />
        </branch>
        <branch name="XLXN_11">
            <wire x2="816" y1="1152" y2="1152" x1="736" />
            <wire x2="736" y1="1152" y2="1248" x1="736" />
            <wire x2="1264" y1="1248" y2="1248" x1="736" />
            <wire x2="1264" y1="1248" y2="1920" x1="1264" />
            <wire x2="816" y1="416" y2="416" x1="768" />
            <wire x2="768" y1="416" y2="512" x1="768" />
            <wire x2="1264" y1="512" y2="512" x1="768" />
            <wire x2="1264" y1="512" y2="592" x1="1264" />
            <wire x2="1264" y1="592" y2="1248" x1="1264" />
            <wire x2="816" y1="1856" y2="1856" x1="768" />
            <wire x2="768" y1="1856" y2="1920" x1="768" />
            <wire x2="1264" y1="1920" y2="1920" x1="768" />
            <wire x2="1264" y1="592" y2="592" x1="1200" />
        </branch>
        <branch name="XLXN_16">
            <wire x2="16" y1="80" y2="1088" x1="16" />
            <wire x2="320" y1="1088" y2="1088" x1="16" />
            <wire x2="816" y1="1088" y2="1088" x1="320" />
            <wire x2="320" y1="1088" y2="1792" x1="320" />
            <wire x2="816" y1="1792" y2="1792" x1="320" />
            <wire x2="1248" y1="80" y2="80" x1="16" />
            <wire x2="1248" y1="80" y2="112" x1="1248" />
            <wire x2="1248" y1="112" y2="1328" x1="1248" />
            <wire x2="752" y1="112" y2="352" x1="752" />
            <wire x2="816" y1="352" y2="352" x1="752" />
            <wire x2="1248" y1="112" y2="112" x1="752" />
            <wire x2="1248" y1="1328" y2="1328" x1="1200" />
        </branch>
        <branch name="XLXN_18">
            <wire x2="816" y1="288" y2="288" x1="784" />
            <wire x2="784" y1="288" y2="1024" x1="784" />
            <wire x2="784" y1="1024" y2="1936" x1="784" />
            <wire x2="800" y1="1936" y2="1936" x1="784" />
            <wire x2="1264" y1="1936" y2="1936" x1="800" />
            <wire x2="1264" y1="1936" y2="2032" x1="1264" />
            <wire x2="816" y1="1024" y2="1024" x1="784" />
            <wire x2="816" y1="1728" y2="1728" x1="800" />
            <wire x2="800" y1="1728" y2="1936" x1="800" />
            <wire x2="1264" y1="2032" y2="2032" x1="1200" />
        </branch>
        <branch name="EN">
            <wire x2="816" y1="960" y2="960" x1="752" />
            <wire x2="752" y1="960" y2="1200" x1="752" />
            <wire x2="1920" y1="1200" y2="1200" x1="752" />
            <wire x2="1920" y1="1200" y2="1568" x1="1920" />
            <wire x2="752" y1="1568" y2="1664" x1="752" />
            <wire x2="816" y1="1664" y2="1664" x1="752" />
            <wire x2="1920" y1="1568" y2="1568" x1="752" />
            <wire x2="816" y1="224" y2="224" x1="800" />
            <wire x2="800" y1="224" y2="480" x1="800" />
            <wire x2="1920" y1="480" y2="480" x1="800" />
            <wire x2="1920" y1="480" y2="1200" x1="1920" />
            <wire x2="1920" y1="224" y2="480" x1="1920" />
        </branch>
        <branch name="CLEAR">
            <wire x2="800" y1="1552" y2="1552" x1="720" />
            <wire x2="816" y1="1552" y2="1552" x1="800" />
            <wire x2="720" y1="1552" y2="2256" x1="720" />
            <wire x2="816" y1="2256" y2="2256" x1="720" />
            <wire x2="800" y1="496" y2="816" x1="800" />
            <wire x2="816" y1="816" y2="816" x1="800" />
            <wire x2="800" y1="816" y2="1552" x1="800" />
            <wire x2="2576" y1="496" y2="496" x1="800" />
            <wire x2="2576" y1="496" y2="1088" x1="2576" />
        </branch>
        <branch name="CLOCK">
            <wire x2="96" y1="848" y2="848" x1="64" />
        </branch>
        <iomarker fontsize="28" x="64" y="848" name="CLOCK" orien="R180" />
        <branch name="TL1">
            <wire x2="96" y1="912" y2="912" x1="64" />
        </branch>
        <iomarker fontsize="28" x="64" y="912" name="TL1" orien="R180" />
        <iomarker fontsize="28" x="1920" y="224" name="EN" orien="R270" />
        <iomarker fontsize="28" x="2576" y="1088" name="CLEAR" orien="R90" />
    </sheet>
</drawing>