//
//  BFDataConnect.cpp
//  Ranotan2D
//
//  Created by talestaka on 2015/03/24.
//
//

#include "BFDataConnect.h"

@implementation BFDataConnect

-(NSData*) getConnectData
{
    NSString* identifer = @"ranotan2d";
    NSURLSessionConfiguration* config = [NSURLSessionConfiguration backgroundSessionConfiguration:identifer];
    NSURLSession* session = [NSURLSession sessionWithConfiguration:config delegate:self delegateQueue:[NSOperationQueue mainQueue]];
    
    
    NSURLRequest* request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://talefantasia.appspot.com/search/plan2?version=2&test=1"]];
    NSURLSessionDownloadTask* download = [session downloadTaskWithRequest:request];
    [download resume];
    return NULL;
    
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location
{
    NSLog(@"%@",@"est");
    NSLog(@"%@",location);
    NSData *data = [NSData dataWithContentsOfURL:location];
    NSArray *array = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingAllowFragments error:nil];
    NSLog(@"%@", [array componentsJoinedByString:@","] );
    
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didResumeAtOffset:(int64_t)fileOffset expectedTotalBytes:(int64_t)expectedTotalBytes
{
    
}

-(void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite
{
    
}

@end
