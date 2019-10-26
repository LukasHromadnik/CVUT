<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="V" />
        <signal name="U" />
        <signal name="T" />
        <signal name="S" />
        <signal name="R" />
        <signal name="Q" />
        <signal name="P" />
        <signal name="D" />
        <signal name="C" />
        <signal name="B" />
        <signal name="A" />
        <port polarity="Output" name="V" />
        <port polarity="Output" name="U" />
        <port polarity="Output" name="T" />
        <port polarity="Output" name="S" />
        <port polarity="Output" name="R" />
        <port polarity="Output" name="Q" />
        <port polarity="Output" name="P" />
        <port polarity="Input" name="D" />
        <port polarity="Input" name="C" />
        <port polarity="Input" name="B" />
        <port polarity="Input" name="A" />
        <blockdef name="sega">
            <timestamp>2014-3-2T12:28:32</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="segb">
            <timestamp>2014-3-2T12:28:48</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="segc">
            <timestamp>2014-3-2T12:29:0</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="segd">
            <timestamp>2014-3-2T12:29:12</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="sege">
            <timestamp>2014-3-2T12:29:24</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="segf">
            <timestamp>2014-3-2T12:29:36</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <blockdef name="segg">
            <timestamp>2014-3-2T12:29:48</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
        </blockdef>
        <block symbolname="sega" name="XLXI_1">
            <blockpin signalname="D" name="d" />
            <blockpin signalname="C" name="c" />
            <blockpin signalname="B" name="b" />
            <blockpin signalname="A" name="a" />
            <blockpin signalname="P" name="f" />
        </block>
        <block symbolname="segf" name="XLXI_6">
            <blockpin signalname="D" name="d" />
            <blockpin signalname="C" name="c" />
            <blockpin signalname="B" name="b" />
            <blockpin signalname="A" name="a" />
            <blockpin signalname="U" name="f" />
        </block>
        <block symbolname="sege" name="XLXI_5">
            <blockpin signalname="D" name="d" />
            <blockpin signalname="C" name="c" />
            <blockpin signalname="B" name="b" />
            <blockpin signalname="A" name="a" />
            <blockpin signalname="T" name="f" />
        </block>
        <block symbolname="segg" name="XLXI_7">
            <blockpin signalname="D" name="d" />
            <blockpin signalname="C" name="c" />
            <blockpin signalname="B" name="b" />
            <blockpin signalname="A" name="a" />
            <blockpin signalname="V" name="f" />
        </block>
        <block symbolname="segd" name="XLXI_4">
            <blockpin signalname="D" name="d" />
            <blockpin signalname="C" name="c" />
            <blockpin signalname="B" name="b" />
            <blockpin signalname="A" name="a" />
            <blockpin signalname="S" name="f" />
        </block>
        <block symbolname="segc" name="XLXI_3">
            <blockpin signalname="D" name="d" />
            <blockpin signalname="C" name="c" />
            <blockpin signalname="B" name="b" />
            <blockpin signalname="A" name="a" />
            <blockpin signalname="R" name="f" />
        </block>
        <block symbolname="segb" name="XLXI_2">
            <blockpin signalname="D" name="d" />
            <blockpin signalname="C" name="c" />
            <blockpin signalname="B" name="b" />
            <blockpin signalname="A" name="a" />
            <blockpin signalname="Q" name="f" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <branch name="V">
            <wire x2="1136" y1="2416" y2="2416" x1="1104" />
        </branch>
        <branch name="U">
            <wire x2="1136" y1="2080" y2="2080" x1="1104" />
        </branch>
        <branch name="T">
            <wire x2="1136" y1="1744" y2="1744" x1="1104" />
        </branch>
        <branch name="S">
            <wire x2="1136" y1="1408" y2="1408" x1="1104" />
        </branch>
        <branch name="R">
            <wire x2="1136" y1="1072" y2="1072" x1="1104" />
        </branch>
        <branch name="Q">
            <wire x2="1136" y1="736" y2="736" x1="1104" />
        </branch>
        <branch name="P">
            <wire x2="1136" y1="400" y2="400" x1="1104" />
        </branch>
        <branch name="D">
            <wire x2="528" y1="288" y2="400" x1="528" />
            <wire x2="528" y1="400" y2="736" x1="528" />
            <wire x2="528" y1="736" y2="1072" x1="528" />
            <wire x2="528" y1="1072" y2="1408" x1="528" />
            <wire x2="528" y1="1408" y2="1744" x1="528" />
            <wire x2="528" y1="1744" y2="2080" x1="528" />
            <wire x2="528" y1="2080" y2="2416" x1="528" />
            <wire x2="720" y1="2416" y2="2416" x1="528" />
            <wire x2="720" y1="2080" y2="2080" x1="528" />
            <wire x2="720" y1="1744" y2="1744" x1="528" />
            <wire x2="720" y1="1408" y2="1408" x1="528" />
            <wire x2="720" y1="1072" y2="1072" x1="528" />
            <wire x2="720" y1="736" y2="736" x1="528" />
            <wire x2="720" y1="400" y2="400" x1="528" />
        </branch>
        <branch name="C">
            <wire x2="448" y1="288" y2="464" x1="448" />
            <wire x2="448" y1="464" y2="800" x1="448" />
            <wire x2="448" y1="800" y2="1136" x1="448" />
            <wire x2="448" y1="1136" y2="1472" x1="448" />
            <wire x2="448" y1="1472" y2="1808" x1="448" />
            <wire x2="448" y1="1808" y2="2144" x1="448" />
            <wire x2="448" y1="2144" y2="2480" x1="448" />
            <wire x2="720" y1="2480" y2="2480" x1="448" />
            <wire x2="720" y1="2144" y2="2144" x1="448" />
            <wire x2="720" y1="1808" y2="1808" x1="448" />
            <wire x2="720" y1="1472" y2="1472" x1="448" />
            <wire x2="720" y1="1136" y2="1136" x1="448" />
            <wire x2="720" y1="800" y2="800" x1="448" />
            <wire x2="720" y1="464" y2="464" x1="448" />
        </branch>
        <branch name="B">
            <wire x2="368" y1="288" y2="528" x1="368" />
            <wire x2="368" y1="528" y2="864" x1="368" />
            <wire x2="368" y1="864" y2="1200" x1="368" />
            <wire x2="368" y1="1200" y2="1536" x1="368" />
            <wire x2="368" y1="1536" y2="1872" x1="368" />
            <wire x2="368" y1="1872" y2="2208" x1="368" />
            <wire x2="368" y1="2208" y2="2544" x1="368" />
            <wire x2="720" y1="2544" y2="2544" x1="368" />
            <wire x2="720" y1="2208" y2="2208" x1="368" />
            <wire x2="720" y1="1872" y2="1872" x1="368" />
            <wire x2="720" y1="1536" y2="1536" x1="368" />
            <wire x2="720" y1="1200" y2="1200" x1="368" />
            <wire x2="720" y1="864" y2="864" x1="368" />
            <wire x2="720" y1="528" y2="528" x1="368" />
        </branch>
        <branch name="A">
            <wire x2="304" y1="288" y2="592" x1="304" />
            <wire x2="304" y1="592" y2="928" x1="304" />
            <wire x2="304" y1="928" y2="1264" x1="304" />
            <wire x2="304" y1="1264" y2="1600" x1="304" />
            <wire x2="304" y1="1600" y2="1936" x1="304" />
            <wire x2="304" y1="1936" y2="2272" x1="304" />
            <wire x2="304" y1="2272" y2="2608" x1="304" />
            <wire x2="720" y1="2608" y2="2608" x1="304" />
            <wire x2="720" y1="2272" y2="2272" x1="304" />
            <wire x2="720" y1="1936" y2="1936" x1="304" />
            <wire x2="720" y1="1600" y2="1600" x1="304" />
            <wire x2="720" y1="1264" y2="1264" x1="304" />
            <wire x2="720" y1="928" y2="928" x1="304" />
            <wire x2="720" y1="592" y2="592" x1="304" />
        </branch>
        <instance x="720" y="624" name="XLXI_1" orien="R0">
        </instance>
        <instance x="720" y="2304" name="XLXI_6" orien="R0">
        </instance>
        <instance x="720" y="1968" name="XLXI_5" orien="R0">
        </instance>
        <instance x="720" y="2640" name="XLXI_7" orien="R0">
        </instance>
        <instance x="720" y="1632" name="XLXI_4" orien="R0">
        </instance>
        <instance x="720" y="1296" name="XLXI_3" orien="R0">
        </instance>
        <instance x="720" y="960" name="XLXI_2" orien="R0">
        </instance>
        <iomarker fontsize="28" x="304" y="288" name="A" orien="R270" />
        <iomarker fontsize="28" x="368" y="288" name="B" orien="R270" />
        <iomarker fontsize="28" x="448" y="288" name="C" orien="R270" />
        <iomarker fontsize="28" x="528" y="288" name="D" orien="R270" />
        <iomarker fontsize="28" x="1136" y="2416" name="V" orien="R0" />
        <iomarker fontsize="28" x="1136" y="2080" name="U" orien="R0" />
        <iomarker fontsize="28" x="1136" y="1744" name="T" orien="R0" />
        <iomarker fontsize="28" x="1136" y="1408" name="S" orien="R0" />
        <iomarker fontsize="28" x="1136" y="1072" name="R" orien="R0" />
        <iomarker fontsize="28" x="1136" y="736" name="Q" orien="R0" />
        <iomarker fontsize="28" x="1136" y="400" name="P" orien="R0" />
    </sheet>
</drawing>