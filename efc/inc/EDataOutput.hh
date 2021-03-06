/*
 * EDataOutput.hh
 *
 *  Created on: 2013-7-29
 *      Author: cxxjava@163.com
 */

#ifndef EDATAOUTPUT_HH_
#define EDATAOUTPUT_HH_

#include "EBase.hh"
#include "EIOException.hh"

namespace efc {

/**
 * The <code>DataOutput</code> interface provides
 * for converting data from any of the Java
 * primitive types to a series of bytes and
 * writing these bytes to a binary stream.
 * There is  also a facility for converting
 * a <code>String</code> into
 * <a href="DataInput.html#modified-utf-8">modified UTF-8</a>
 * format and writing the resulting series
 * of bytes.
 * <p>
 * For all the methods in this interface that
 * write bytes, it is generally true that if
 * a byte cannot be written for any reason,
 * an <code>IOException</code> is thrown.
 *
 * @version 1.23, 05/23/06
 * @see     java.io.DataInput
 * @see     java.io.DataOutputStream
 * @since   JDK1.0
 */

interface EDataOutput : virtual public EObject {
	virtual ~EDataOutput() {
	}

	/**
	 * Writes to the output stream the eight
	 * low-order bits of the argument <code>b</code>.
	 * The 24 high-order  bits of <code>b</code>
	 * are ignored.
	 *
	 * @param      b   the byte to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void write(int b) THROWS(EIOException) = 0;

	/**
	 * Writes <code>len</code> bytes from array
	 * <code>b</code>, in order,  to
	 * the output stream.  If <code>b</code>
	 * is <code>null</code>, a <code>NullPointerException</code>
	 * is thrown.  If <code>off</code> is negative,
	 * or <code>len</code> is negative, or <code>off+len</code>
	 * is greater than the length of the array
	 * <code>b</code>, then an <code>IndexOutOfBoundsException</code>
	 * is thrown.  If <code>len</code> is zero,
	 * then no bytes are written. Otherwise, the
	 * byte <code>b[off]</code> is written first,
	 * then <code>b[off+1]</code>, and so on; the
	 * last byte written is <code>b[off+len-1]</code>.
	 *
	 * @param      b     the data.
	 * @param      len   the number of bytes to write.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void write(const void* b, int len) THROWS(EIOException) = 0;

	/**
	 * Writes a <code>boolean</code> value to this output stream.
	 * If the argument <code>v</code>
	 * is <code>true</code>, the value <code>(byte)1</code>
	 * is written; if <code>v</code> is <code>false</code>,
	 * the  value <code>(byte)0</code> is written.
	 * The byte written by this method may
	 * be read by the <code>readBoolean</code>
	 * method of interface <code>DataInput</code>,
	 * which will then return a <code>boolean</code>
	 * equal to <code>v</code>.
	 *
	 * @param      v   the boolean to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void writeBoolean(boolean v) THROWS(EIOException) = 0;

	/**
	 * Writes to the output stream the eight low-
	 * order bits of the argument <code>v</code>.
	 * The 24 high-order bits of <code>v</code>
	 * are ignored. (This means  that <code>writeByte</code>
	 * does exactly the same thing as <code>write</code>
	 * for an integer argument.) The byte written
	 * by this method may be read by the <code>readByte</code>
	 * method of interface <code>DataInput</code>,
	 * which will then return a <code>byte</code>
	 * equal to <code>(byte)v</code>.
	 *
	 * @param      v   the byte value to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void writeByte(int v) THROWS(EIOException) = 0;

	/**
	 * Writes two bytes to the output
	 * stream to represent the value of the argument.
	 * The byte values to be written, in the  order
	 * shown, are: <p>
	 * <pre><code>
	 * (byte)(0xff &amp; (v &gt;&gt; 8))
	 * (byte)(0xff &amp; v)
	 * </code> </pre> <p>
	 * The bytes written by this method may be
	 * read by the <code>readShort</code> method
	 * of interface <code>DataInput</code> , which
	 * will then return a <code>short</code> equal
	 * to <code>(short)v</code>.
	 *
	 * @param      v   the <code>short</code> value to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void writeShort(int v) THROWS(EIOException) = 0;

	/**
	 * Writes an <code>int</code> value, which is
	 * comprised of four bytes, to the output stream.
	 * The byte values to be written, in the  order
	 * shown, are:
	 * <p><pre><code>
	 * (byte)(0xff &amp; (v &gt;&gt; 24))
	 * (byte)(0xff &amp; (v &gt;&gt; 16))
	 * (byte)(0xff &amp; (v &gt;&gt; &#32; &#32;8))
	 * (byte)(0xff &amp; v)
	 * </code></pre><p>
	 * The bytes written by this method may be read
	 * by the <code>readInt</code> method of interface
	 * <code>DataInput</code> , which will then
	 * return an <code>int</code> equal to <code>v</code>.
	 *
	 * @param      v   the <code>int</code> value to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void writeInt(int v) THROWS(EIOException) = 0;

	/**
	 * Writes a <code>long</code> value, which is
	 * comprised of eight bytes, to the output stream.
	 * The byte values to be written, in the  order
	 * shown, are:
	 * <p><pre><code>
	 * (byte)(0xff &amp; (v &gt;&gt; 56))
	 * (byte)(0xff &amp; (v &gt;&gt; 48))
	 * (byte)(0xff &amp; (v &gt;&gt; 40))
	 * (byte)(0xff &amp; (v &gt;&gt; 32))
	 * (byte)(0xff &amp; (v &gt;&gt; 24))
	 * (byte)(0xff &amp; (v &gt;&gt; 16))
	 * (byte)(0xff &amp; (v &gt;&gt;  8))
	 * (byte)(0xff &amp; v)
	 * </code></pre><p>
	 * The bytes written by this method may be
	 * read by the <code>readLong</code> method
	 * of interface <code>DataInput</code> , which
	 * will then return a <code>long</code> equal
	 * to <code>v</code>.
	 *
	 * @param      v   the <code>long</code> value to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void writeLLong(llong v) THROWS(EIOException) = 0;

	/**
	 * Writes a <code>float</code> value,
	 * which is comprised of four bytes, to the output stream.
	 * It does this as if it first converts this
	 * <code>float</code> value to an <code>int</code>
	 * in exactly the manner of the <code>Float.floatToIntBits</code>
	 * method  and then writes the <code>int</code>
	 * value in exactly the manner of the  <code>writeInt</code>
	 * method.  The bytes written by this method
	 * may be read by the <code>readFloat</code>
	 * method of interface <code>DataInput</code>,
	 * which will then return a <code>float</code>
	 * equal to <code>v</code>.
	 *
	 * @param      v   the <code>float</code> value to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void writeFloat(float v) THROWS(EIOException) = 0;

	/**
	 * Writes a <code>double</code> value,
	 * which is comprised of eight bytes, to the output stream.
	 * It does this as if it first converts this
	 * <code>double</code> value to a <code>long</code>
	 * in exactly the manner of the <code>Double.doubleToLongBits</code>
	 * method  and then writes the <code>long</code>
	 * value in exactly the manner of the  <code>writeLong</code>
	 * method. The bytes written by this method
	 * may be read by the <code>readDouble</code>
	 * method of interface <code>DataInput</code>,
	 * which will then return a <code>double</code>
	 * equal to <code>v</code>.
	 *
	 * @param      v   the <code>double</code> value to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void writeDouble(double v) THROWS(EIOException) = 0;

	/**
	 * Writes a string to the output stream.
	 * For every character in the string
	 * <code>s</code>,  taken in order, one byte
	 * is written to the output stream.  If
	 * <code>s</code> is <code>null</code>, a <code>NullPointerException</code>
	 * is thrown.<p>  If <code>s.length</code>
	 * is zero, then no bytes are written. Otherwise,
	 * the character <code>s[0]</code> is written
	 * first, then <code>s[1]</code>, and so on;
	 * the last character written is <code>s[s.length-1]</code>.
	 * For each character, one byte is written,
	 * the low-order byte, in exactly the manner
	 * of the <code>writeByte</code> method . The
	 * high-order eight bits of each character
	 * in the string are ignored.
	 *
	 * @param      s   the string of bytes to be written.
	 * @throws     IOException  if an I/O error occurs.
	 */
	virtual void writeBytes(const char* s) THROWS(EIOException) = 0;
};

} /* namespace efc */
#endif /* EDATAOUTPUT_HH_ */
