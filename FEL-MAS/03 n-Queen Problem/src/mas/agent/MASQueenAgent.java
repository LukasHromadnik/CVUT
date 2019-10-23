package mas.agent;

import java.util.LinkedList;
import java.util.List;

import cz.agents.alite.communication.Communicator;
import cz.agents.alite.communication.Message;
import cz.agents.alite.communication.MessageHandler;
import cz.agents.alite.communication.content.Content;

public abstract class MASQueenAgent implements MessageHandler, Runnable {

	interface OnAgentFinishedCallback {
		void agentFinishedSuccess(int agentId, int finalColumn);
		void agentFinishedFailure();
	}

	int agentId;
	Communicator communicator;
	List<String> agents = new LinkedList<String>();
	List<Message> inbox = new LinkedList<Message>();

	private OnAgentFinishedCallback agentFinishedCallback;
	private int nAgents;
	public boolean terminated = false;

	public MASQueenAgent(int agentId, int nAgents) {
		this.agentId = agentId;
		this.nAgents = nAgents;
	}

	public void run() {
		start(agentId, nAgents);

		while (!terminated) {
			List<Message> newMessages = getNewMessages();
			if (!newMessages.isEmpty()) {
				processMessages(newMessages);
			}

			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {}
		}
	}

	@Override
	public synchronized void notify(Message msg) {
		inbox.add(msg);
	}

	protected synchronized List<Message> getNewMessages() {
		LinkedList<Message> newMessages = new LinkedList<Message>(inbox);
		inbox = new LinkedList<Message>();
		return newMessages;
	}

	public void setCommunicator(Communicator communicator, List<String> agents) {
		this.communicator = communicator;
		this.agents = new LinkedList<String>(agents);
		this.nAgents = agents.size();
	}

	protected void sendMessage(String receiver, Content content) {
		Message msg = communicator.createMessage(content);
		LinkedList<String> receivers = new LinkedList<String>();
		receivers.add(receiver);
		msg.addReceivers(receivers);
		communicator.sendMessage(msg);
	}

	protected void broadcast(Content content) {
		Message msg = communicator.createMessage(content);
		LinkedList<String> receivers = new LinkedList<String>();
		for (String agent : agents) {
			if (!agent.equals(Integer.toString(agentId))) {
				receivers.add(agent);
			}
		}
		msg.addReceivers(receivers);
		communicator.sendMessage(msg);
	}

	abstract protected void start(int agentId, int nAgents);

	abstract protected void processMessages(List<Message> newMessages);

	public void registerAgentFinishedCallback(OnAgentFinishedCallback callback) {
		this.agentFinishedCallback = callback;
	}

	protected void notifySolutionFound(int finalColumn) {
		terminated = true;
		if (this.agentFinishedCallback != null) {
			this.agentFinishedCallback.agentFinishedSuccess(agentId,
					finalColumn);
		}
	}

	protected void notifySolutionDoesNotExist() {
		terminated = true;
		if (this.agentFinishedCallback != null) {
			this.agentFinishedCallback.agentFinishedFailure();
		}
	}
	
	protected int nAgents() {
		return nAgents;
	}
	
	public int getAgentId() {
		return agentId;
	}

}