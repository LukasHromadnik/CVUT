package mas.agents.task.mining;

import mas.agents.Message;
import mas.agents.TaskEnvironment;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Random;
import java.util.concurrent.ConcurrentLinkedQueue;

public class Environment implements TaskEnvironment, MouseListener {

    private MapConfig config;
    private ArrayList<ConcurrentLinkedQueue<CommandMessage>> eventQueues;
    private long[] queueEnds;
    private Point2D[] positions;
    private boolean[] carriesGold;

    JFrame frame;
    JPanel panel;

    Random random = new Random();

    public Environment(MapConfig config) {
        this.config = config;

        eventQueues = new ArrayList<>();
        for(int i = 0 ; i <= config.getAgentsCount() ; i++) {
            eventQueues.add(new ConcurrentLinkedQueue<>());
        }

        queueEnds = new long[config.getAgentsCount() + 1];
        positions = new Point2D[config.getAgentsCount() + 1];
        carriesGold = new boolean[config.getAgentsCount() + 1];

        for(int i = 1; i <= config.getAgentsCount() ; i++) {
            Position p = config.getInitialPositions().get(i-1);
            positions[i] = new Point2D.Double(p.x, p.y);
        }

        if(Config.GUI) {
            frame = new JFrame();
            panel = new JPanel() {
                @Override
                public void paint(Graphics _g) {
                    synchronized (config) {
                        Graphics2D g = (Graphics2D) _g;
                        g.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

                        g.setColor(Color.WHITE);
                        g.fillRect(0, 0, getWidth(), getHeight());

                        g.setColor(Color.BLACK);
                        for (int i = 1; i < config.width; i++) {
                            final double x = i * panel.getWidth() / (double) config.width;
                            g.draw(new Line2D.Double(x, 0, x, getHeight()));
                        }
                        for (int i = 1; i < config.height; i++) {
                            final double y = i * panel.getHeight() / (double) config.height;
                            g.draw(new Line2D.Double(0, y, getWidth(), y));
                        }

                        double width = panel.getWidth() / (double) config.width;
                        double height = panel.getHeight() / (double) config.height;

                        for (Position p : config.getObstacles()) {
                            g.fill(new Rectangle2D.Double(
                                    p.x * getWidth() / (double) config.width,
                                    p.y * getHeight() / (double) config.height,
                                    width, height));
                        }

                        g.setColor(Color.green);
                        for (Position p : config.getDepots()) {
                            g.fill(new Rectangle2D.Double(
                                    p.x * getWidth() / (double) config.width,
                                    p.y * getHeight() / (double) config.height,
                                    width, height));
                        }

                        g.setColor(Color.YELLOW);
                        for (HashMap.Entry<Position, Integer> entry : config.getGolds()) {
                            Position p = entry.getKey();
                            g.fill(new Rectangle2D.Double(
                                    p.x * getWidth() / (double) config.width + 0.1 * width,
                                    p.y * getHeight() / (double) config.height + 0.1 * height,
                                    0.8 * width, 0.8 * height));
                        }

                        g.setColor(Color.BLACK);
                        for (HashMap.Entry<Position, Integer> entry : config.getGolds()) {
                            Position p = entry.getKey();
                            Integer count = entry.getValue();
                            ctext(g, Integer.toString(count),
                                    p.x * getWidth() / (double) config.width + 0.1 * width,
                                    p.y * getHeight() / (double) config.height + 0.1 * height);
                        }

                        for (int i = 1; i <= config.getAgentsCount(); i++) {
                            g.setColor(carriesGold[i] ? Color.ORANGE : Color.BLUE);
                            g.fill(new Ellipse2D.Double(
                                    positions[i].getX() * getWidth() / config.width,
                                    positions[i].getY() * getHeight() / config.height,
                                    width, height));
                        }

                        g.setColor(Color.WHITE);
                        FontMetrics fm = g.getFontMetrics();
                        for (int i = 1; i <= config.getAgentsCount(); i++) {
                            String s = Integer.toString(i);
                            ctext(g, Integer.toString(i),
                                    (positions[i].getX() + 0.5) * getWidth() / config.width,
                                    (positions[i].getY() + 0.5) * getHeight() / config.height);
                        }
                    }
                }

                private void ctext(Graphics2D g, String s, double _x, double _y) {
                    FontMetrics fm = g.getFontMetrics();
                    double x = _x - fm.stringWidth(s)/2;
                    double y = _y - fm.getHeight()/2 + fm.getAscent();
                    g.drawString(s, (int)x, (int)y);
                }
            };

            frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
            frame.getContentPane().setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.PAGE_AXIS));
            panel.setDoubleBuffered(true);
            panel.addMouseListener(this);

            frame.add(new JLabel(config.getName()));

            frame.add(panel);
            frame.setVisible(true);
            frame.setSize(700, 700);

            addSlider("Mean delay", 0, 9000, (int)(1000*Math.log10(Config.MEAN)), new ChangeListener() {
                @Override
                public void stateChanged(ChangeEvent changeEvent) {
                    JSlider slider = (JSlider)changeEvent.getSource();
                    double vFraction = Config.VARIANCE / Config.MEAN;
                    Config.MEAN = Math.pow(10, slider.getValue() / 1000.0);
                    Config.VARIANCE = vFraction * Config.MEAN;
                }
            });
            addSlider("Variance", 0, 500, (int)(1000*Config.VARIANCE/Config.MEAN), new ChangeListener() {
                @Override
                public void stateChanged(ChangeEvent changeEvent) {
                    JSlider slider = (JSlider)changeEvent.getSource();
                    double vFraction = slider.getValue() / 1000.0;
                    Config.VARIANCE = vFraction * Config.MEAN;
                }
            });
        }

        new Thread() {
            @Override
            public void run() {
                try {
                    Environment.this.run();
                } catch(Exception e) {
                    e.printStackTrace();
                }
            }
        }.start();
    }

    private JSlider addSlider(String label, int min, int max, int value, ChangeListener listener) {
        frame.add(new JLabel(label));

        JSlider slider = new JSlider(SwingConstants.HORIZONTAL, min, max, value);
        slider.addChangeListener(listener);
        frame.add(slider);

        return slider;
    }

    public void processMessage(Message _m) {
        if(!(_m instanceof CommandMessage)) return;
        CommandMessage m = (CommandMessage) _m;

        if(m instanceof StatusRequestMessage) {
            m.setDuration(0);
        } else {
            m.setDuration(sampleDuration());
        }
        eventQueues.get(m.getSender()).offer(m);
    }

    public void run() {
        long last = System.nanoTime();
        long begin = System.currentTimeMillis();
        boolean reported = false;

        try {
            while (true) {
                synchronized (config) {
                    long current = System.nanoTime();
                    long elapsed = current - last;
                    last = current;

                    for (int i = 1; i <= config.getAgentsCount(); i++) {
                        long cElapsed = elapsed;
                        ConcurrentLinkedQueue<CommandMessage> queue = eventQueues.get(i);

                        while (!queue.isEmpty() && cElapsed > 0) {
                            CommandMessage cmd = queue.peek();
                            long remains = cmd.duration - cmd.elapsed;
                            cmd.elapsed += cElapsed;
                            cElapsed -= remains;

                            if (cmd.origin == null) cmd.origin = positions[i];

                            if (activate(cmd) || cmd.elapsed >= cmd.duration) {
                                eventQueues.get(i).poll();
                                sendStatus(cmd);
                            } else {
                                break;
                            }
                        }
                    }
                }

                if (frame != null) {
                    panel.repaint();
                }

                if(!reported && config.getGolds().isEmpty()) {
                    reported = true;
                    System.out.printf("All golds collected in %.1fs\n", (System.currentTimeMillis()-begin)/1000.0);
                }
            }
        } catch(Exception e) {
            e.printStackTrace();
            try {
                synchronized (this) {
                    wait();
                }
            } catch(InterruptedException ie) {}
        }
    }

    public boolean activate(CommandMessage cmd) {
        if(cmd instanceof GrabMessage) {
            if(carriesGold[cmd.getSender()]) return true;

            if(!checkNeighbor(cmd.getSender())) {
                throw new RuntimeException("Noone to help :-(");
            }

            if(cmd.elapsed >= cmd.duration) {
                int x = (int)Math.round(cmd.origin.getX());
                int y = (int)Math.round(cmd.origin.getY());

                carriesGold[cmd.getSender()] = config.pick(x, y);
                return true;
            }
        } else if(cmd instanceof DropMessage) {
            if(!carriesGold[cmd.getSender()]) return true;

            if(cmd.elapsed >= cmd.duration) {
                int x = (int)Math.round(cmd.origin.getX());
                int y = (int)Math.round(cmd.origin.getY());

                config.drop(x, y);
                carriesGold[cmd.getSender()] = false;
                return true;
            }
        } else if(cmd instanceof MoveMessage) {
            double completion = Math.min(cmd.duration, cmd.elapsed) / (double)cmd.duration;
            MoveMessage move = (MoveMessage)cmd;

            if(occupied(new Position((int)Math.round(move.origin.getX() + move.dx), (int)Math.round(move.origin.getY() + move.dy)), cmd.getSender())) {
                setPosition(cmd.getSender(), move.origin);
                return true;
            }

            if(cmd.elapsed >= cmd.duration) {
                setPosition(cmd.getSender(), new Point2D.Double(Math.round(move.origin.getX() + move.dx), Math.round(move.origin.getY() + move.dy)));
                return true;
            } else {
                setPosition(cmd.getSender(), new Point2D.Double(move.origin.getX() + completion * move.dx, move.origin.getY() + completion * move.dy));
            }
        }

        return false;
    }

    private boolean checkNeighbor(int agent) {
        for(int i = 1 ; i <= config.getAgentsCount() ; i++) {
            if(i == agent) continue;

            Point2D agentPosition = positions[i];
            Point2D currentAgentPosition = positions[agent];

            if (agentPosition.distanceSq(currentAgentPosition) <= 1 + 1e-6) return true;

            //else if (positions[i].distanceSq(positions[agent]) <= 1+1e-6) return true;
        }
        return false;
    }

    private boolean occupied(Position pos, int agentQuery) {
        if(pos.x < 0 || pos.x >= config.width || pos.y < 0 || pos.y >= config.height) return true;
        if(config.isObstacle(pos.x, pos.y)) return true;
        for(int i = 1 ; i <= config.getAgentsCount() ; i++) {
            if(agentQuery == i) continue;
            if(positions[i].distanceSq(pos.x, pos.y) < 1-1e-6) return true;
        }
        return false;
    }

    private long sampleDuration() {
        return (long)(random.nextGaussian()*Config.VARIANCE + Config.MEAN);
    }

    private void sendStatus(Message m) throws IOException {
        int x = (int)Math.round(positions[m.getSender()].getX());
        int y = (int)Math.round(positions[m.getSender()].getY());
        ArrayList<StatusMessage.SensorData> sensorData = new ArrayList<>();
        for(int dx = -1; dx <= 1 ; dx++) {
            for(int dy = -1 ; dy <= 1 ; dy++) {
                if(config.isObstacle(x+dx, y+dy)) {
                    sensorData.add(new StatusMessage.SensorData(x+dx, y+dy, StatusMessage.OBSTACLE));
                }
                if(config.isDepot(x+dx, y+dy)) {
                    sensorData.add(new StatusMessage.SensorData(x+dx, y+dy, StatusMessage.DEPOT));
                }
                if(config.isGold(x+dx, y+dy)) {
                    for(int i = 0 ; i < config.numGolds(x+dx, y+dy) ; i++) {
                        sensorData.add(new StatusMessage.SensorData(x+dx, y+dy, StatusMessage.GOLD));
                    }
                }
                for(int i = 1 ; i <= config.getAgentsCount() ; i++) {
                    if(i == m.getSender()) continue;
                    if(positions[i].distanceSq(x+dx, y+dy) < 1e-6) {
                        sensorData.add(new StatusMessage.SensorData(x+dx, y+dy, StatusMessage.AGENT));
                    }
                }
            }
        }

        m.replyWith(new StatusMessage(x, y, config.width, config.height, sensorData));
    }

    private void setPosition(int agentId, Point2D p) {
        positions[agentId] = p;
    }

    @Override
    public void mouseClicked(MouseEvent mouseEvent) {
        if(mouseEvent.getButton() == MouseEvent.BUTTON1) {
            int x = config.width * mouseEvent.getX() / panel.getWidth();
            int y = config.height * mouseEvent.getY() / panel.getHeight();

            config.addGold(x, y);
        }
    }

    @Override
    public void mousePressed(MouseEvent mouseEvent) {
    }

    @Override
    public void mouseReleased(MouseEvent mouseEvent) {
    }

    @Override
    public void mouseEntered(MouseEvent mouseEvent) {
    }

    @Override
    public void mouseExited(MouseEvent mouseEvent) {
    }
}
