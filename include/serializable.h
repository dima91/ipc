/**
 * \file serializable.h
 * \brief
 * \author Luca Di Mauro
 */

#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H


namespace ipc {

	/**
	 * \class Serializable
	 * \brief
	 */
	class Serializable {
	public :
		Serializable () {}
		virtual ~Serializable ()	= 0;


		unsigned char *serialize	();

		template<typename T>
		T *deserialize	(unsigned char *data, int len);
	};
} // namespace ipc


#endif // SERIALIZABLE_H