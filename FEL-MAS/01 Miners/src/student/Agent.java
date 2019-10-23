package student;

import mas.agents.AbstractAgent;
import mas.agents.Message;
import mas.agents.SimulationApi;
import mas.agents.task.mining.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Random;

public class Agent extends AbstractAgent {

    private static final int MOVE_UP = 0;
    private static final int MOVE_RIGHT = 1;
    private static final int MOVE_DOWN = 2;
    private static final int MOVE_LEFT = 3;

    // Helper variable for the desired destination
    private Position destination = null;

    // Responses from the help request
    private ArrayList<HelpResponseMessage> responses = new ArrayList<>();

    // Helper variable for sending direct messages
    private int senderID = 0;

    // Current agent state according to the environment
    private AgentState state = AgentState.SENSING;
    private AgentState previousState = AgentState.SENSING;

    private boolean hasGold = false;
    private boolean isOnlySearching = true;

    // Game environment
    private GameEnvironment environment;

    // Current agent status
    private StatusMessage status = null;

    public Agent(int id, InputStream is, OutputStream os, SimulationApi api) throws IOException, InterruptedException {
        super(id, is, os, api);
    }

    @Override
    public void act() throws Exception {
        while (true) {
            handleMessages();

            switch (state) {
                case SENSING:
                    status = sense();
                    environment = new GameEnvironment(status);
                    break;
                case SEARCHING:
                    if (destination == null) {
                        destination = getRandomUndiscoveredPosition();
                    }
                    status = getNextDirectionMove();
                    break;
                case GOING_TO_HELP:
                case GOING_FOR_GOLD:
                    status = getNextDirectionMove();
                    break;
                case GOING_TO_DEPOT:
                    if (environment.depot != null) {
                        destination = environment.depot;
                    } else if (destination == null) {
                        destination = getRandomUndiscoveredPosition();
                    }
                    status = getNextDirectionMove();
                    break;
                case PICKING_UP:
                    status = pick();
                    hasGold = true;
                    break;
                case DROPPING:
                    status = drop();
                    hasGold = false;
                    break;
                default:
                    status = sense();
                    break;
            }

            updateEnvironment();

            updateState();

            try {
                Thread.sleep(200);
                //if (getAgentId() == 1) {
                //    System.out.println(environment);
                //}
                //System.out.println(String.format("[AGENT %d] %s", getAgentId(), destination));
            }
            catch (InterruptedException ie) { }
        }
    }

    /**
     * Incoming messages handler
     *
     * @throws Exception From sending message
     */
    private void handleMessages() throws Exception {
        while (messageAvailable()) {
            Message m = readMessage();

            //if (! (m instanceof EnvironmentUpdateMessage)) {
            //    log("I have received " + m);
            //}

            if (m instanceof EnvironmentUpdateMessage) {
                environment.updateWith((EnvironmentUpdateMessage) m);
            } else if (m instanceof HelpRequestMessage) {
                switch (state) {
                    case CALLING_FOR_HELP:
                    case WAITING_FOR_HELP:
                    case PICKING_UP:
                    case DROPPING:
                    case GOING_TO_HELP:
                    case HELPING:
                        sendMessage(m.getSender(), HelpResponseMessage.cannot(status.agentX, status.agentY));
                        break;
                    default:
                        sendMessage(m.getSender(), HelpResponseMessage.can(status.agentX, status.agentY));
                        break;
                }
            } else if (m instanceof HelpResponseMessage) {
                responses.add((HelpResponseMessage) m);
            } else if (m instanceof GoldHelpMessage) {
                if (state == AgentState.GOING_TO_HELP) {
                    sendMessage(m.getSender(), new ResetHelpMessage());
                    return;
                }

                // Remove reservation
                if (destination != null && state == AgentState.GOING_FOR_GOLD) {
                    EnvironmentUpdateMessage reservationRemoveMessage = new EnvironmentUpdateMessage(GameEnvironment.RESERVED, destination.x, destination.y, true);
                    environment.updateWith(reservationRemoveMessage);
                    broadcastMessage(reservationRemoveMessage);

                    EnvironmentUpdateMessage goldReservationMessage = new EnvironmentUpdateMessage(StatusMessage.GOLD, destination.x, destination.y);
                    environment.updateWith(goldReservationMessage);
                    broadcastMessage(goldReservationMessage);
                }

                destination = ((GoldHelpMessage) m).getPosition();
                updateAgentState(AgentState.GOING_TO_HELP);
                senderID = m.getSender();
                isOnlySearching = false;
            } else if (m instanceof PickMessage) {
                updateAgentState(AgentState.PICKING_UP);
                senderID = m.getSender();
            } else if (m instanceof ThankYouMessage) {
                senderID = 0;
                destination = null;
            } else if (m instanceof ResetHelpMessage) {
                responses.clear();
                state = AgentState.CALLING_FOR_HELP;
                sendHelpRequestIfNeeded();
            }
        }
    }

    /**
     * Get the next move for the agent when state is SEARCHING
     *
     * @return Next possible move
     * @throws IOException From sending message
     */
    private StatusMessage getNextSearchMove() throws IOException {
        ArrayList<Integer> possibleMoves = new ArrayList<>();

        if (status.agentX > 0) {
            Position leftPosition = new Position(status.agentX - 1, status.agentY);
            if (isPossibleMoveTo(leftPosition)) {
                possibleMoves.add(MOVE_LEFT);
            }
        }

        if (status.agentX + 1 < status.width) {
            Position rightPosition = new Position(status.agentX + 1, status.agentY);
            if (isPossibleMoveTo(rightPosition)) {
                possibleMoves.add(MOVE_RIGHT);
            }
        }

        if (status.agentY > 0) {
            Position upPosition = new Position(status.agentX, status.agentY - 1);
            if (isPossibleMoveTo(upPosition)) {
                possibleMoves.add(MOVE_UP);
            }
        }

        if (status.agentY + 1 < status.height) {
            Position downPosition = new Position(status.agentX, status.agentY + 1);
            if (isPossibleMoveTo(downPosition)) {
                possibleMoves.add(MOVE_DOWN);
            }
        }

        if (possibleMoves.size() == 0) return sense();

        Random random = new Random();
        int index = random.nextInt(possibleMoves.size());

        switch (possibleMoves.get(index)) {
            case MOVE_UP: return up();
            case MOVE_DOWN: return down();
            case MOVE_LEFT: return left();
            case MOVE_RIGHT: return right();
            default: return sense();
        }
    }

    /**
     * Get the next move to get to the desired destination
     *
     * @return Next move in the direction of the desired destination
     * @throws IOException From sending message
     */
    private StatusMessage getNextDirectionMove() throws IOException {
        ArrayList<Integer> possibleMoves = new ArrayList<>();

        if (destination == null) return getNextSearchMove();

        if (status.agentX > destination.x) {
            Position leftPosition = new Position(status.agentX - 1, status.agentY);
            if (isPossibleMoveTo(leftPosition)) {
                possibleMoves.add(MOVE_LEFT);
            }
        }

        if (status.agentX < destination.x) {
            Position rightPosition = new Position(status.agentX + 1, status.agentY);
            if (isPossibleMoveTo(rightPosition)) {
                possibleMoves.add(MOVE_RIGHT);
            }
        }

        if (status.agentY > destination.y) {
            Position upPosition = new Position(status.agentX, status.agentY - 1);
            if (isPossibleMoveTo(upPosition)) {
                possibleMoves.add(MOVE_UP);
            }
        }

        if (status.agentY < destination.y) {
            Position downPosition = new Position(status.agentX, status.agentY + 1);
            if (isPossibleMoveTo(downPosition)) {
                possibleMoves.add(MOVE_DOWN);
            }
        }

        if (possibleMoves.size() == 1 || possibleMoves.size() == 0) {
            System.out.printf("[AGENT %d] (%d) %d", getAgentId(), possibleMoves.size(), (possibleMoves.isEmpty()) ? -1 : possibleMoves.get(0));
            try {
                logStatus(status);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        Position agentPosition = new Position(status.agentX, status.agentY);
        if (possibleMoves.size() == 0 && state == AgentState.GOING_TO_HELP && agentPosition.distanceFrom(destination) <= 2) return sense();
        if (possibleMoves.size() == 0) return getNextSearchMove();

        /*
        if (possibleMoves.size() == 1) {
            try {
                logStatus(status);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
*/
        Random random = new Random();
        int index = random.nextInt(possibleMoves.size());

        switch (possibleMoves.get(index)) {
            case MOVE_UP:
                return up();
            case MOVE_DOWN:
                return down();
            case MOVE_LEFT:
                return left();
            case MOVE_RIGHT:
                return right();
            default:
                return sense();
        }
    }

    /**
     * Updates environment with information given from the sensors
     *
     * @throws IOException From sending message
     */
    private void updateEnvironment() throws IOException {
        for (StatusMessage.SensorData data : status.sensorInput) {
            switch (data.type) {
                case StatusMessage.DEPOT:
                    if (environment.depot != null) continue;

                    EnvironmentUpdateMessage depotUpdateMessage = new EnvironmentUpdateMessage(StatusMessage.DEPOT, data.x, data.y);
                    broadcastMessage(depotUpdateMessage);
                    environment.updateWith(depotUpdateMessage);
                    break;
                case StatusMessage.GOLD:
                    EnvironmentUpdateMessage goldUpdateMessage = new EnvironmentUpdateMessage(StatusMessage.GOLD, data.x, data.y);

                    if (environment.isReserved(goldUpdateMessage.getPosition())) continue;

                    if (environment.shouldUpdateWith(goldUpdateMessage)) {
                        broadcastMessage(goldUpdateMessage);
                        environment.updateWith(goldUpdateMessage);
                    }

                    if (isOnlySearching && !hasGold) {
                        EnvironmentUpdateMessage goldReservationMessage = new EnvironmentUpdateMessage(GameEnvironment.RESERVED, data.x, data.y);
                        destination = goldReservationMessage.getPosition();
                        environment.updateWith(goldReservationMessage);
                        broadcastMessage(goldReservationMessage);
                        isOnlySearching = false;
                    }
                    break;
                case StatusMessage.OBSTACLE:
                    EnvironmentUpdateMessage obstacleUpdateMessage = new EnvironmentUpdateMessage(StatusMessage.OBSTACLE, data.x, data.y);
                    if (environment.shouldUpdateWith(obstacleUpdateMessage)) {
                        broadcastMessage(obstacleUpdateMessage);
                        environment.updateWith(obstacleUpdateMessage);
                    }
                    break;
                default:
                    break;
            }
        }

        for (int i = 0; i < 6; i++) {
            Position discoverPosition;
            if (i < 3) {
                discoverPosition = new Position(status.agentX - 1 + i, status.agentY);
            } else {
                discoverPosition = new Position(status.agentX, status.agentY - 1 + i % 3);
            }

            if (environment.isDiscovered(discoverPosition)) continue;
            environment.setDiscovered(discoverPosition);
            broadcastMessage(new EnvironmentUpdateMessage(0, discoverPosition.x, discoverPosition.y));
        }
    }

    /**
     * Update state handler
     */
    private void updateState() throws IOException {
        Position agentPosition = new Position(status.agentX, status.agentY);
        switch (state) {
            case SENSING:
                updateAgentState(AgentState.SEARCHING);
                break;
            case SEARCHING:
                if (isOnlySearching) {
                    if (destination != null && agentPosition.isOn(destination) && environment.typeAt(destination) == GameEnvironment.GameObjectType.RESERVED) {
                        updateAgentState(AgentState.CALLING_FOR_HELP);
                        sendHelpRequestIfNeeded();
                    } else if (destination == null || agentPosition.isOn(destination)) {
                        destination = getRandomUndiscoveredPosition();
                    }
                } else {
                    updateAgentState(AgentState.GOING_FOR_GOLD);
                }
                break;
            case GOING_FOR_GOLD:
                if ( ! agentPosition.isOn(destination)) break;
                updateAgentState(AgentState.CALLING_FOR_HELP);
                sendHelpRequestIfNeeded();
                break;
            case CALLING_FOR_HELP:
                callForHelpIfPossible();
                break;
            case WAITING_FOR_HELP:
                break;
            case PICKING_UP:
                updateAgentState(AgentState.GOING_TO_DEPOT);
                sendMessage(senderID, new ThankYouMessage());

                EnvironmentUpdateMessage removeGoldMessage = new EnvironmentUpdateMessage(StatusMessage.GOLD, agentPosition.x, agentPosition.y, true);
                environment.updateWith(removeGoldMessage);
                broadcastMessage(removeGoldMessage);
                destination = null;
                break;
            case GOING_TO_DEPOT:
                if (destination == null) break;
                if (agentPosition.isOn(destination) && environment.typeAt(destination) != GameEnvironment.GameObjectType.DEPOT) {
                    destination = getRandomUndiscoveredPosition();
                } else if (agentPosition.isNextTo(destination) && environment.typeAt(destination) != GameEnvironment.GameObjectType.DEPOT) {
                    destination = getRandomUndiscoveredPosition();
                } else if (agentPosition.isOn(destination) && environment.typeAt(destination) == GameEnvironment.GameObjectType.DEPOT) {
                    updateAgentState(AgentState.DROPPING);
                }
                break;
            case DROPPING:
                updateAgentState(AgentState.SEARCHING);
                destination = null;
                isOnlySearching = true;
                senderID = 0;
                break;
            case GOING_TO_HELP:
                if (destination == null) break;
                if ( ! agentPosition.isNextTo(destination)) break;
                //System.out.println(String.format("%s - %s", agentPosition, destination));
                sendMessage(senderID, new PickMessage());
                updateAgentState(AgentState.HELPING);
            case HELPING:
                if (senderID != 0) break;
                updateAgentState(AgentState.SEARCHING);
                isOnlySearching = true;
            default:
                break;
        }
    }

    private void sendHelpRequestIfNeeded() throws IOException {
        if (responses.size() != 0) return;
        broadcastMessage(new HelpRequestMessage());
    }

    private void callForHelpIfPossible() throws IOException {
        if (responses.size() != 3) return;

        int minDistance = Integer.MAX_VALUE;
        int minSender = 0;
        Position agentPosition = new Position(status.agentX, status.agentY);

        for (HelpResponseMessage m : responses) {
            if ( ! m.value) continue;
            int distance = agentPosition.distanceFrom(m.getPosition());
            if (distance > minDistance) continue;
            minDistance = distance;
            minSender = m.getSender();
        }

        if (minSender == 0) {
            responses.clear();
            sendHelpRequestIfNeeded();
            return;
        }

        sendMessage(minSender, new GoldHelpMessage(status.agentX, status.agentY));
        updateAgentState(AgentState.WAITING_FOR_HELP);

        responses.clear();
    }

    ///////////////////////////////////////////// HELPERS /////////////////////////////////////////////

    /**
     * Method for broadcasting message
     *
     * @param message Given message
     * @throws IOException From sending message
     */
    private void broadcastMessage(Message message) throws IOException {
        for (int i = 1; i <= 4; i++) {
            if (i == this.getAgentId()) continue;
            sendMessage(i, message);
        }
    }

    /**
     * Log current agent status
     *
     * @param statusMessage Given status
     * @throws Exception From logging
     */
    private void logStatus(StatusMessage statusMessage) throws Exception {
        String[] types = { "", "obstacle", "depot", "gold", "agent" };
        log(String.format("I am now on position [%d,%d] of a %dx%d map.", statusMessage.agentX, statusMessage.agentY, statusMessage.width, statusMessage.height));
        for (StatusMessage.SensorData data : statusMessage.sensorInput) {
            log(String.format("I see %s at [%d,%d]", types[data.type], data.x, data.y));
        }
    }

    /**
     * Helper methods for saving previous agent's state
     *
     * @param newState New state of the agent
     */
    private void updateAgentState(AgentState newState) {
        previousState = state;

        if (newState == AgentState.SEARCHING && hasGold) {
            state = AgentState.GOING_TO_DEPOT;
        } else {
            state = newState;
        }

        System.out.println(String.format("[Agent %d] %s -> %s", getAgentId(), previousState, state));
    }

    private boolean isPossibleMoveTo(Position position) {
        for (StatusMessage.SensorData data : status.sensorInput) {
            Position dataPosition = new Position(data.x, data.y);
            if ( ! dataPosition.isOn(position)) continue;
            switch (data.type) {
                case StatusMessage.OBSTACLE:
                case StatusMessage.AGENT:
                    return false;
                default:
                    return true;
            }
        }

        return true;
    }

    private Position getRandomUndiscoveredPosition() {
        return environment.getRandomUndiscoveredPosition();
    }

}
