//===========================================================================
/*!
 * 
 *
 * \brief       Definition of an OpenML Dataset.
 * 
 * 
 * 
 *
 * \author      T. Glasmachers
 * \date        2016
 *
 *
 * \par Copyright 1995-2016 Shark Development Team
 * 
 * <BR><HR>
 * This file is part of Shark.
 * <http://image.diku.dk/shark/>
 * 
 * Shark is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published 
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Shark is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Shark.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
//===========================================================================

#ifndef SHARK_OPENML_DATASET_H
#define SHARK_OPENML_DATASET_H

#include "PooledEntity.h"
#include "CachedFile.h"


namespace shark {
namespace openML {


/// \brief Representation of an OpenML data set.
SHARK_EXPORT_SYMBOL class Dataset : public PooledEntity<Dataset>
{
private:
	friend class PooledEntity<Dataset>;

	/// \brief Construct an existing dataset from its ID.
	Dataset(IDType id, bool downloadData = false);

public:
	////////////////////////////////////////////////////////////
	// tagging
	//

	/// \brief Add a tag to the entity.
	void tag(std::string const& tagname);

	/// \brief Remove a tag from the entity.
	void untag(std::string const& tagname);

	////////////////////////////////////////////////////////////
	// printing
	//

	/// \brief Print a human readable summary of the entity.
	void print(std::ostream& os = std::cout) const;

	////////////////////////////////////////////////////////////
	// property getters
	//

	/// \brief Name of the data set.
	std::string const& name() const
	{ return m_name; }

	/// \brief Textual description of the data set.
	std::string const& description() const
	{ return m_description; }

	/// \brief Default target for prediction; ignore - obtain the target feature from the task instead.
	std::string const& defaultTargetFeature() const
	{ return m_defaultTargetFeature; }

	/// \brief Data set file format, should be ARFF for now.
	std::string const& format() const
	{ return m_format; }

	/// \brief 
	std::string const& status() const
	{ return m_status; }

	/// \brief Data set visibility (e.g., public or private).
	std::string const& visibility() const
	{ return m_visibility; }

	/// \brief Number of features of the data set as found in the ARFF data set file.
	std::size_t numberOfFeatures() const
	{ return m_feature.size(); }

	/// \brief Obtain a feature description by index.
	FeatureDescription const& feature(std::size_t index) const
	{ return m_feature[index]; }

	/// \brief Obtain the index of a feature by name.
	std::size_t featureIndex(std::string const& name) const
	{
		for (std::size_t i=0; i<m_feature.size(); i++)
		{
			if (m_feature[i].name == name) return i;
		}
		throw SHARKEXCEPTION("[featureIndex] feature " + name + " not found");
	}

	/// \brief Obtain the underlying ARFF data set file
	CachedFile const& datafile() const
	{ return m_file; }

private:
	// properties
	std::string m_name;                   ///< name of the data set
	std::string m_description;            ///< textual description of the data set
	std::string m_defaultTargetFeature;   ///< default target for prediction; ignore - obtain the target feature from the task instead
	std::string m_format;                 ///< data set file format, should be ARFF for now
	std::string m_licence;                ///< license, "public domain" if empty
	std::string m_status;                 ///< data set status (e.g., active)
	std::string m_visibility;             ///< data set visibility (e.g., public or private)

	// features
	std::vector<FeatureDescription> m_feature;   ///< feature meta data

	// external file
	CachedFile m_file;                    ///< underlying ARFF data set file
};


};  // namespace openML
};  // namespace shark
#endif
