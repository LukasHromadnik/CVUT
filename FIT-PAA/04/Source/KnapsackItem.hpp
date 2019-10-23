struct KnapsackItem {
	int m_Price;
	int m_Volume;
	double getRatio() const;

	KnapsackItem();
	
	bool operator < (const KnapsackItem& other) const {
		return (this->getRatio() < other.getRatio());
	}
};

// ------------------------------------------------------------------------------------------------

KnapsackItem::KnapsackItem() {
	m_Price = 0;
	m_Volume = 0;
}

double KnapsackItem::getRatio() const {
	return (double)m_Price / (double)m_Volume;
}
