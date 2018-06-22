#ifndef PRIORITYARRAY_H
#define PRIORITYARRAY_H

#include <vector>
#include <algorithm>
#ifdef KGE_USE_BOOST
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#endif // KGE_USE_BOOST

namespace kge
{
	namespace core
	{

		template<typename ValueType, typename PriorityType>
		class PriorityArray
		{
		public:
			struct Item
			{
				ValueType val;
				PriorityType pri;

				Item() : val(0), pri(0) {}
				Item(const ValueType& val, const PriorityType& pri) : val(val), pri(pri) {}

				bool operator<(const Item& other) const { return this->pri < other.pri; }

				bool operator==(const Item& other) const { return this->val == other.val; }
			};

			//! Constructor
			PriorityArray(): m_iIndex(-1)
			{

			}

			void Add(const ValueType& val, const PriorityType& pri)
			{
				m_items.push_back(Item(val, pri));
			}

			void Sort()
			{
				std::sort(m_items.begin(), m_items.end());
			}

			const ValueType& operator[](size_t index) const
			{
				return m_items[index].val;
			}

			const ValueType& Get(size_t index, PriorityType& priority) const
			{
				priority = m_items[index].pri;
				return m_items[index].val;
			}

			void Remove(const ValueType& val, const PriorityType& pri)
			{
				typename std::vector<Item>::iterator it = std::find(m_items.begin(), m_items.end(), Item(val, pri));

				if (it != m_items.end())
				{
					m_items.erase(it);
				}
			}

			void Clear()
			{
				m_items.clear();
			}

			size_t Size() const
			{
				return m_items.size();
			}

			int intSize() const
			{
				return (int)m_items.size();
			}

			ValueType Next()
			{
				m_iIndex++;
				if (m_items.size() == 0)
					return 0;
				if (m_iIndex >= m_items.size())
					return 0;

				return m_items[m_iIndex].val;
			}

		private:
			std::vector<Item>	m_items;
			int					m_iIndex;

#ifdef KGE_USE_BOOST
			friend class boost::serialization::access;
			// Serialize
			template<class Archive>
			void serialize(Archive & ar, const unsigned int file_version)
			{
				ar & m_items;
			}
#endif // KGE_USE_BOOST

		};

	} // core

} // kge

#endif // PRIORITYARRAY_H
