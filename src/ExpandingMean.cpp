
ExpandingMean::ExpandingMean(int rows, int cols) {
	m_sum = dlib::matrix<double>(rows, cols);
	dlib::set_all_elements(m_sum, NAN);
	m_count = 0;
}

void ExpandingMean::consume(const dlib::matrix<double> &x) {
	if (!dlib::is_finite(x)) {
		return;
	}

	if (m_count == 0) {
		m_sum = x;
	} else {
		m_sum = m_sum + x;
	}
	++m_count;
}

dlib::matrix<double> ExpandingMean::value() const {
	return (1. / m_count) * m_sum;
}
