package mas.agent.student;

import java.util.*;

import cz.agents.alite.communication.content.Content;
import mas.agent.MASQueenAgent;
import cz.agents.alite.communication.Message;

public class MyQueenAgent extends MASQueenAgent {

	private Integer value = 0;
	private Integer[] agentView = new Integer[nAgents()];
	private ArrayList<Integer[]> nogoodList = new ArrayList<>();

    public MyQueenAgent(int agentId, int nAgents) {
		// Leave this method as it is...
    	super(agentId, nAgents);
	}


	@Override
	protected void start(int agentId, int nAgents) {

		// This method is called when the agent is initialized.
		// e.g., you can start sending messages:

        for (int i = 0; i < nAgents(); i++) {
            agentView[i] = -1;
        }

		notifySuccessors(new OkContent(this.value));
	}

    @Override
	protected void processMessages(List<Message> newMessages) {
		// This method is called whenever there are any new messages for the robot
		boolean gotNogood = false;
		Integer[] checkForSolution = null;
		// You can list the messages like this:
        for (Message message : newMessages) {
            if (message.getContent() instanceof OkContent) {
                OkContent content = (OkContent) message.getContent();
                agentView[Integer.parseInt(message.getSender())] = content.intContent();
                System.out.println("[OK] A" + getAgentId() + " A" + message.getSender() + " " + message.getContent().toString());
            } else if (message.getContent() instanceof NogoodContent) {
                Integer[] nogoodContent = ((NogoodContent) message.getContent()).agentView();
                nogoodList.add(nogoodContent);
                System.out.println("[NOGOOD] A" + getAgentId() + " A" + message.getSender() + "\n" + message.getContent().toString());
                gotNogood = true;
            } else if (message.getContent() instanceof SendSolutionContent) {
                notifySolutionFound(this.value);
                return;
            } else if (message.getContent() instanceof NoSolutionContent) {
                notifySolutionDoesNotExist();
                return;
            } else if (message.getContent() instanceof CheckViewContent) {
                checkForSolution = ((CheckViewContent) message.getContent()).agentView();
            }
        }

        checkAgentView(gotNogood);
        if (checkForSolution != null) {
            checkViewForSolution(checkForSolution);
        }

        // You can slow down the search process like this:
        //try {
        //    Thread.sleep(100);
        //} catch (InterruptedException e) {}
	}

	private void notifySuccessors(Content content) {
        for (Integer i = getAgentId() + 1; i < nAgents(); i++) {
            sendMessage(i.toString(), content);
        }
    }

    private void checkAgentView(boolean gotNogood) {
        boolean isInconsistent = false;

        ArrayList<Integer> possibleValues = new ArrayList<>();
        for (int i = 0; i < nAgents(); i++) {
            possibleValues.add(i);
        }

        for (Integer[] nogood : nogoodList) {
            if ( ! isNogoodValid(nogood, false)) continue;
            int bannedPosition = nogood[getAgentId()];
            possibleValues.remove(new Integer(bannedPosition));
        }

        for (int i = 0; i < getAgentId(); i++) {
            if (agentView[i] == -1) continue;
            if ( ! isValueConsistentWithAgent(i)) isInconsistent = true;
            possibleValues.remove(agentView[i]);

            int diagonalValue1 = getAgentId() - i + agentView[i];
            if (diagonalValue1 < nAgents()) {
                possibleValues.remove(new Integer(diagonalValue1));
            }

            int diagonalValue2 = agentView[i] - getAgentId() + i;
            if (diagonalValue2 >= 0) {
                possibleValues.remove(new Integer(diagonalValue2));
            }
        }

        if ( ! possibleValues.contains(this.value)) {
            isInconsistent = true;
        }

        if (isInconsistent && possibleValues.size() > 0) {
            // RANDOM SELECTION
            // Random random = new Random();
            // int index = random.nextInt(possibleValues.size());
            // this.value = possibleValues.get(index);

            // ORDERED SELECTION
            this.value = possibleValues.get(0);

            System.out.println("[CHANGE POSITION] A" + String.valueOf(getAgentId()) + " " + value.toString());
            notifySuccessors(new OkContent(this.value));

            if (getAgentId() == nAgents() - 1) {
                Integer[] solutionAgentView = Arrays.copyOf(agentView, agentView.length);
                solutionAgentView[getAgentId()] = value;
                checkViewForSolution(solutionAgentView);
            }
        } else if (isInconsistent) {
            backtrack(gotNogood);
        } else if (gotNogood) {
            notifySuccessors(new OkContent(this.value));
        } else {
            if (getAgentId() == nAgents() - 1) {
                Integer[] solutionAgentView = Arrays.copyOf(agentView, agentView.length);
                solutionAgentView[getAgentId()] = value;
                checkViewForSolution(solutionAgentView);
            }
        }
    }

    private void backtrack(boolean gotNogood) {
        if (getAgentId() == 0) {
            notifySolutionDoesNotExist();
            System.out.println("[SOLUTION] Not found");
            broadcast(new NoSolutionContent());
            return;
        }

        int nogoodReceiver = getAgentId() - 1;
        for (int i = getAgentId() - 1; i >= 0; i--) {
            if (agentView[i] == -1) continue;
            nogoodReceiver = i;
            break;
        }

        System.out.println("[BACKTRACK] A" + String.valueOf(getAgentId()) + " A" + String.valueOf(nogoodReceiver) + " " + agentView[nogoodReceiver].toString());
        Integer[] agentViewCopy = Arrays.copyOf(agentView, agentView.length);
        sendMessage(String.valueOf(nogoodReceiver), new NogoodContent(agentViewCopy));
        agentView[nogoodReceiver] = -1;

        checkAgentView(gotNogood);
    }

    private boolean isValueConsistentWithAgent(int agent) {
        ArrayList<Integer> agentValues = agentEffectForCurrentAgent(agent);
        return !agentValues.contains(this.value);
    }

    private ArrayList<Integer> agentEffectForCurrentAgent(int agent) {
        return agentEffectForAgent(agent, getAgentId());
    }

    private ArrayList<Integer> agentEffectForAgent(int parent, int current) {
        Integer agentValue = agentView[parent];
        int diff = Math.abs(current - parent);

        ArrayList<Integer> values = new ArrayList<>();

        Integer left = agentValue - diff;
        if (left > 0) {
            values.add(left);
        }

        values.add(agentValue);

        Integer right = agentValue + diff;
        if (right < nAgents()) {
            values.add(right);
        }

        return values;
    }

    private boolean isNogoodValid(Integer[] content, boolean checkCurrentValue) {
        boolean isValid = true;
        for (int agent = 0; agent < getAgentId(); agent++) {
            if (content[agent].equals(agentView[agent])) continue;
            isValid = false;
        }

        if (checkCurrentValue && ! content[getAgentId()].equals(this.value)) {
            isValid = false;
        }

        return isValid;
    }

    private void printAgentView() {
        String result = "[AGENT " + getAgentId() + "]\n";
        for (int i = 0; i < 2 * nAgents() + 1; i++) {
            result += "-";
        }
        result += "\n";
        for (int i = 0; i < nAgents(); i++) {
            result += "|";
            for (int j = 0; j < nAgents(); j++) {
                if (agentView[i] == j) {
                    result += "Q|";
                } else if (i == getAgentId() && j == this.value) {
                    result += "*|";
                } else {
                    result += " |";
                }
            }
            result += "\n";
        }
        for (int i = 0; i < 2 * nAgents() + 1; i++) {
            result += "-";
        }
        result += "\n";

        System.out.println(result);
    }

    private void checkViewForSolution(Integer[] solutionAgentView) {
        System.out.println("[SOLUTION] Checking");
        for (int i = 0; i < getAgentId(); i++) {
            if (agentView[i] == -1 || agentView[i] != solutionAgentView[i]) return;
        }

        if (value != solutionAgentView[getAgentId()]) return;

        System.out.println("[SOLUTION] AgentView equality");

        if (getAgentId() > 0) {
            sendMessage(String.valueOf(getAgentId() - 1), new CheckViewContent(agentView));
        } else {
            broadcast(new SendSolutionContent());
            notifySolutionFound(value);
        }
    }

}
