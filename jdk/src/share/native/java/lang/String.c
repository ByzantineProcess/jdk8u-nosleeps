/*
 * Copyright (c) 1997, 1998, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#include "jvm.h"
#include "java_lang_String.h"
#include <stdio.h>  // For printf and related functions

// Function to log strings to stdout
void log_string_to_stdout(JNIEnv *env, jobject str_obj) {
    jclass string_class = (*env)->GetObjectClass(env, str_obj);
    jmethodID get_bytes_method = (*env)->GetMethodID(env, string_class, "getBytes", "()[B");
    
    if (get_bytes_method == NULL) {
        fprintf(stderr, "Error: Could not find method 'getBytes'\n");
        return;
    }

    jbyteArray byte_array = (jbyteArray)(*env)->CallObjectMethod(env, str_obj, get_bytes_method);
    if (byte_array == NULL) {
        fprintf(stderr, "Error: Could not get byte array\n");
        return;
    }

    jsize length = (*env)->GetArrayLength(env, byte_array);
    jbyte *byte_elements = (*env)->GetByteArrayElements(env, byte_array, NULL);
    
    if (byte_elements == NULL) {
        fprintf(stderr, "Error: Could not get byte array elements\n");
        return;
    }

    // Convert byte array to a C string (assuming UTF-8 encoding)
    char *c_str = (char *)malloc(length + 1);
    if (c_str == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        (*env)->ReleaseByteArrayElements(env, byte_array, byte_elements, 0);
        return;
    }

    memcpy(c_str, byte_elements, length);
    c_str[length] = '\0';  // Null-terminate the string

    // Log the string to stdout
    printf("Interning string: %s\n", c_str);

    // Clean up
    free(c_str);
    (*env)->ReleaseByteArrayElements(env, byte_array, byte_elements, 0);
}

JNIEXPORT jobject JNICALL
Java_java_lang_String_intern(JNIEnv *env, jobject this)
{
    // Log the string before interning
    log_string_to_stdout(env, this);
    
    // Proceed with the original functionality
    return JVM_InternString(env, this);
}
